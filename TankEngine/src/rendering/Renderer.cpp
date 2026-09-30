#include <glad/glad.h>
#include <glm/gtc/matrix_inverse.hpp>
#include <nodes/interfaces/MeshContainer.h>
#include <components/CubeMap.h>
#include <components/Model.h>
#include <components/Light.h>
#include <scene/Scene.h>
#include <scene/Camera.h>
#include <Shader.h>
#include <Texture.h>
#include "Renderer.h"


namespace Tank
{
	void Renderer::drawMesh(const Shader &shader, const Mesh &mesh)
	{
		unsigned int diffuseIdx = 0;
		unsigned int specularIdx = 0;

		shader.use();

		for (unsigned int i = 0; i < mesh.m_textures.size(); i++)
		{
			glActiveTexture(GL_TEXTURE0 + i);

			std::string name = mesh.m_textures[i]->getTexType();
			std::string number;

			if (name == "diffuse")
				number = std::to_string(diffuseIdx++);
			else if (name == "specular")
				number = std::to_string(specularIdx++);

			shader.setInt("material." + name + "[" + number + "]", i);

			glBindTexture(GL_TEXTURE_2D, mesh.m_textures[i]->getTexID());
		}

		// Draw mesh vertices
		glBindVertexArray(mesh.m_vao);
		glDrawElements(GL_TRIANGLES, mesh.m_indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		shader.unuse();
	}


	/// <summary>
	/// Sets up the stencil buffer to write all fragments rendered
	/// after this is called (to the stencil buffer).
	/// 
	/// Make sure to render (draw) the object after this is called.
	/// </summary>
	void Renderer::beginEditorOutline(const IMeshContainer &outlined)
	{
		if (!outlined.m_outlineEnabled) return;

		// Pass or discard? All frags pass, as GL_ALWAYS is used.
		glStencilFunc(GL_ALWAYS, 1, 0xFF);
		// Each bit is written to the stencil buffer as is.
		glStencilMask(0xFF);
	}


	/// <summary>
	/// After an object is drawn, disable stencil writing and depth
	/// testing. Then draw a scaled-up version of the object, in
	/// a block colour.
	/// </summary>
	void Renderer::endEditorOutline(TransformComponent &transform, const IMeshContainer &outlined, const Camera &camera)
	{
		if (!outlined.m_outlineEnabled) return;

		// Only draw parts of the object that are outside of the drawn
		// shape. If concealed, this will include the entire drawn shape.
		glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
		// Disable stencil writing and depth testing
		glStencilMask(0x00);

		outlined.m_outlineShader->use(); // use

		// Setup uniforms
		const glm::vec3 scale = transform.Scale;
		transform.Scale = scale * 1.025f;
		outlined.m_outlineShader->setMat4("PVM", camera.m_projection * camera.m_view * transform.getWorldModelMatrix());
		transform.Scale = scale;

		/// <summary>
		/// Draw all the meshes in the object, with outline shader enabled and transform
		/// temporarily scaled up.
		/// </summary>
		for (unsigned i = 0; i < outlined.m_meshes.size(); i++)
		{
			drawMesh(*outlined.m_outlineShader, *outlined.m_meshes[i]);
		}

		// Now disable writing to the stencil buffer.
		glStencilMask(0xFF);
		glStencilFunc(GL_ALWAYS, 0, 0xFF);
	}


	void Renderer::drawCubeMap(CubeMapComponent *cubeMap, const Camera &camera)
	{
		const Shader &shader = cubeMap->getShader();
		shader.use();

		// Bind all owned texture objects
		int texTarget = cubeMap->m_texture->getTexTarget();
		int texID = cubeMap->m_texture->getTexID();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(texTarget, texID);

		shader.setMat4("view", glm::mat4(glm::mat3(camera.m_view)));
		shader.setMat4("proj", camera.m_projection);

		glDepthMask(GL_FALSE);
		glBindVertexArray(cubeMap->m_vao);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		glDepthMask(GL_TRUE);

		shader.unuse();
	}


	void Renderer::drawModel(TransformComponent &transform, const ModelComponent &model, const Camera &camera)
	{
		glCullFace(model.m_cullFace);

		beginEditorOutline(model);
		const Shader &shader = model.getShader();

		shader.use();
		shader.setVec3("tex_scale", glm::vec3{ 1, 1, 1 });
		shader.setFloat("material.Ns", 32.0f);

		auto P = camera.m_projection;
		auto V = camera.m_view;
		auto M = transform.getWorldModelMatrix();
		auto VM = V * M;

		shader.setMat4("PVM", P * VM);
		shader.setMat4("VM", VM);
		shader.setMat4("V", V);
		shader.setMat4("VM_it", glm::inverseTranspose(VM));

		// Process lights
		{
			auto scene = Scene::getActiveScene();
			auto activeLights = scene->getLights();

			const Shader &shader = model.getShader();
			for (LightComponent *light : activeLights)
			{
				light->updateShader(shader);
			}

			shader.setInt("num_dir_lights", scene->getNumLights(LightType::Directional));
			shader.setInt("num_point_lights", scene->getNumLights(LightType::Point));
		}

		for (unsigned i = 0; i < model.m_meshes.size(); i++)
		{
			drawMesh(shader, *model.m_meshes[i]);
		}
		shader.unuse();

		endEditorOutline(transform, model, camera);
		glCullFace(GL_BACK);
	}
}