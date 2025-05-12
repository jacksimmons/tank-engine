#include <optional>
#include <imgui.h>
#include <glm/gtx/euler_angles.hpp>
#include "transform.hpp"
#include "colours.hpp"
#include "file.hpp"
#include "widget.hpp"
#include "shader.hpp"
#include "nodes/node.hpp"
#include "nodes/scene.hpp"
#include "nodes/light.hpp"
#include "nodes/camera.hpp"
#include "nodes/sprite.hpp"
#include "nodes/model.hpp"
#include "nodes/ui/console.hpp"
#include "nodes/ui/inspector.hpp"
#include "nodes/ui/file_dialog.hpp"
#include "nodes/interfaces/editor_only.hpp"
#include "nodes/interfaces/shader_container.hpp"


namespace Tank::Editor
{
	_Inspector::_Inspector(const std::string &name) : _Window(name, ImGuiWindowFlags_None, false)
	{
		m_inspectedNode = nullptr;
	}


	void _Inspector::drawPanel()
	{
		if (m_inspectedNode)
		{
			ImGui::TextColored(Tank::Colour::TITLE, "Type");
			ImGui::Text(typeid(*m_inspectedNode).name());

			// Prevent users from modifying editor nodes, which would likely cause a crash.
			if (m_inspectedNode->isEditorControlled())
			{
				ImGui::Text("Modifying the editor is not supported.");
			}
			else
			{
				drawNodeSection();
			}

			// Draw Node subclass sections
			if (Tank::Scene *scene = dynamic_cast<Tank::Scene *>(m_inspectedNode))
				drawSceneSection(scene);

			if (Tank::Camera *camera = dynamic_cast<Tank::Camera *>(m_inspectedNode))
				drawCameraSection(camera);

			if (Tank::Light *light = dynamic_cast<Tank::Light *>(m_inspectedNode))
				drawLightSection(light);

			if (Tank::Sprite *sprite = dynamic_cast<Tank::Sprite *>(m_inspectedNode))
				drawSpriteSection(sprite);

			if (Tank::Model *model = dynamic_cast<Tank::Model *>(m_inspectedNode))
				drawModelSection(model);

			if (Tank::IShaderContainer *shaders = dynamic_cast<Tank::IShaderContainer *>(m_inspectedNode))
				drawShaderSection(shaders);

			if (Tank::IMeshContainer *meshes = dynamic_cast<Tank::IMeshContainer *>(m_inspectedNode))
				drawMeshSection(meshes);
		}	
	}

	
	/// <summary>
	/// Draws inspector section that is present for all Nodes.
	/// </summary>
	void _Inspector::drawNodeSection()
	{
		Tank::Transform *transform = m_inspectedNode->getTransform();
		const glm::mat4 &modelMatrix = transform->getWorldModelMatrix();

		bool enabled = m_inspectedNode->getEnabled();
		if (ImGui::Checkbox("Enabled", &enabled))
			m_inspectedNode->setEnabled(enabled);

		bool visible = m_inspectedNode->getVisibility();
		if (ImGui::Checkbox("Visible", &visible))
			m_inspectedNode->setVisibility(visible);

		ImGui::TextColored(Tank::Colour::TITLE, "Name");
		Tank::Widget::textInput("##Inspector_Name", m_inspectedNode->getName(),
			[this](const std::string &newName)
			{
				m_inspectedNode->setName(newName);
			}
		);

		if (ImGui::Button("<Snap To>"))
		{
			auto cam = Tank::Scene::getActiveScene()->getActiveCamera();
			if (cam != nullptr)
			{
				glm::mat4 worldMatrix = transform->getWorldModelMatrix();
				cam->setPosition(Tank::mat4::getTranslation(worldMatrix));
				cam->setRotation(Tank::mat4::getRotation(worldMatrix));
			}
		}

		ImGui::TextColored(Tank::Colour::TITLE, "Model Matrix");
		// glm::mat4 indexing is column-major, but ImGui is row-major.
		// Transposing the model means an ImGui row corresponds to a model matrix row.
		glm::mat4 displayMatrix = glm::transpose(modelMatrix);
		for (int i = 0; i < 4; i++)
		{
			glm::vec4 row = displayMatrix[i];
			std::string rowText = std::to_string(row.x) + "\t" +
				std::to_string(row.y) + "\t" +
				std::to_string(row.z) + "\t" +
				std::to_string(row.w);
			ImGui::Text(rowText.c_str());
		}

		ImGui::TextColored(Colour::TITLE, "Translation");
		Widget::vec3Input(
			"##Inspector_Translation",
			transform->getLocalTranslation(),
			[&transform](glm::vec3 newTranslation)
			{
				transform->setLocalTranslation(newTranslation);
			}
		);

		ImGui::TextColored(Colour::TITLE, "Scale");
		Widget::vec3Input(
			"##Inspector_Scale",
			transform->getLocalScale(),
			[&transform](glm::vec3 newScale)
			{
				transform->setLocalScale(newScale);
			}
		);

		ImGui::TextColored(Colour::TITLE, "Rotation (Euler Angles)");
		Widget::vec3Input(
			"##Inspector_Rotation_EulerAngles",
			glm::eulerAngles(transform->getLocalRotation()),
			[&transform](glm::vec3 newRotation)
			{
				transform->setLocalRotation(quat::fromAngleAxis(newRotation));
			}
		);
	}


	/// <summary>
	/// Draws a section specifically for the root node.
	/// </summary>
	void _Inspector::drawSceneSection(Scene *scene)
	{
		ImGui::TextColored(Colour::TITLE, "Active Camera");

		// Determine active camera text name and colour.
		std::string cameraName;
		ImColor cameraNameCol;
		Camera *activeCamera = scene->getActiveCamera();
		if (activeCamera)
		{
			cameraName = activeCamera->getName();
			cameraNameCol = Colour::NORMAL;
		}
		else
		{
			cameraName = "NULL";
			cameraNameCol = Colour::DISABLED;
		}
		
		ImGui::SetNextItemAllowOverlap();
		ImGui::TextColored(cameraNameCol, cameraName.c_str());
		ImGui::SameLine();

		// Display a button to change the active camera.
		// Clicking on it loads a list of all Camera nodes descending from this scene.
		if (ImGui::SmallButton("Set##INSPECTOR_SCENE_SET_CAM"))
		{
			ImGui::OpenPopup("##INSPECTOR_SCENE_SET_CAM_LIST");
		}

		if (ImGui::BeginPopup("##INSPECTOR_SCENE_SET_CAM_LIST"))
		{
			scene->forEachDescendant(
				[&scene, this](Node *node)
				{
					if (Camera *cam = dynamic_cast<Camera *>(node))
					{
						if (ImGui::Button((cam->getPath() + "##INSPECTOR_SCENE_SET_CAM_LIST_BTN").c_str()))
						{
							scene->setActiveCamera(cam);
						}
					}
				}
			);

			ImGui::EndPopup();
		}
	}


	/// <summary>
	/// Draws inspector section that is present for all nodes with shaders.
	/// </summary>
	void _Inspector::drawShaderSection(IShaderContainer *shaders)
	{
		const ShaderSources &sources = shaders->getShader()->getShaderSources();
		ShaderSources copy = ShaderSources(sources);

		{
			std::string loc = drawShaderSourceSection("Vertex", copy.vertex);
			if (!loc.empty())
			{
				copy.vertex.location = loc;
			}
		}

		{
			std::string loc = drawShaderSourceSection("Fragment", copy.fragment);
			if (!loc.empty())
			{
				copy.fragment.location = loc;
			}
		}

		{
			std::string loc = drawShaderSourceSection("Geometry", copy.geometry);
			if (!loc.empty())
			{
				copy.geometry.location = loc;
			}
		}

		if (copy != sources)
		{
			auto maybeShader = Shader::createShader(copy);
			if (maybeShader.has_value())
			{
				shaders->setShader(std::move(maybeShader.value()));
			}
		}
	}


	/// <summary>
	/// Draws section for a single shader source (e.g. frag shader).
	/// </summary>
	std::string _Inspector::drawShaderSourceSection(const std::string &sourceName, ShaderSource &source)
	{
		std::string retPath;

		std::string shaderType = sourceName + " shader";
		ImGui::TextColored(Colour::TITLE, shaderType.c_str());
		Widget::textInput(("##Inspector_" + shaderType).c_str(), source.location.string(),
			[&retPath] (const std::string &newPath)
			{
				retPath = newPath;
			}
		);

		std::string shaderContents;
		if (File::readLines("shaders" / source.location, shaderContents) == File::ReadResult::Success)
		{
			// https://github.com/ocornut/imgui/issues/2429
			ImGui::TextUnformatted(shaderContents.c_str());
		}

		return retPath;
	}


	void _Inspector::drawMeshSection(IMeshContainer *meshContainer)
	{
		if (ImGui::CollapsingHeader("Show Meshes##Inspector_Meshes"))
		{
			// Togglable info about all meshes
			auto meshes = meshContainer->getMeshes();
			for (int i = 0; i < meshes.size(); i++)
			{
				// Togglable info about this mesh
				auto vertices = meshes[i]->getVertices();
				if (ImGui::CollapsingHeader(std::string{ "Mesh " + std::to_string(i) + " (" + std::to_string(vertices.size()) + " vertices)##Inspector_Mesh" }.c_str()))
				{
					// Display info about this mesh
					auto indices = meshes[i]->getIndices();

					for (unsigned index : indices)
					{
						Vertex vertexThisIndex = vertices[index];
						ImGui::Text("Position: "); ImGui::SameLine();
						ImGui::Text(glm::to_string(vertexThisIndex.position).c_str()); ImGui::SameLine();
						ImGui::Text("Normal: "); ImGui::SameLine();
						ImGui::Text(glm::to_string(vertexThisIndex.normal).c_str()); ImGui::SameLine();
						ImGui::Text("TexCoords: "); ImGui::SameLine();
						ImGui::Text(glm::to_string(vertexThisIndex.texCoords).c_str());
					}
				}
			}
		}
	}


	/// <summary>
	/// Draws inspector section that is present for all Cameras.
	/// </summary>
	void _Inspector::drawCameraSection(Camera *camera)
	{
		ImGui::TextColored(Colour::TITLE, "Camera Eye");
		glm::vec3 eye = camera->getTransformedEye();
		ImGui::Text(glm::to_string(eye).c_str());

		ImGui::TextColored(Colour::TITLE, "Camera Centre");
		glm::vec3 centre = camera->getTransformedCentre();
		ImGui::Text(glm::to_string(centre).c_str());

		ImGui::TextColored(Colour::TITLE, "Camera Up");
		glm::vec3 up = camera->getTransformedUp();
		ImGui::Text(glm::to_string(up).c_str());

		ImGui::TextColored(Colour::TITLE, "Camera Pan Speed");
		Widget::floatInput("##Inspector_Camera_PanSpd", camera->getPanSpeed(), [camera](float modified) { camera->setPanSpeed(modified); });

		ImGui::TextColored(Colour::TITLE, "Camera Rotation Speed");
		Widget::floatInput("##Inspector_Camera_RotSpd", camera->getRotSpeed(), [camera](float modified) { camera->setRotSpeed(modified); });

		ImGui::TextColored(Colour::TITLE, "Culling Distance");
		ImGui::Text("Near"); ImGui::SameLine();
		Widget::floatInput("##Inspector_Camera_CullNear", camera->getCullNear(), [camera](float modified) { camera->setCullNear(modified); });
		ImGui::Text("Far"); ImGui::SameLine();
		Widget::floatInput("##Inspector_Camera_CullFar", camera->getCullFar(), [camera](float modified) { camera->setCullFar(modified); });
	}


	void _Inspector::drawLightSection(Light *light)
	{
		ImGui::TextColored(Colour::TITLE, "Light Struct");
		std::string lightStruct = light->getLightStruct();
		ImGui::Text(lightStruct.c_str());

		ImGui::TextColored(Colour::TITLE, "Ambient Intensity (RGB)");
		Widget::vec3Input(
			"##Inspector_Light_Ambient",
			light->getAmbient(),
			[&light](glm::vec3 newAmbient)
			{
				light->setAmbient(newAmbient);
			}
		);

		ImGui::TextColored(Colour::TITLE, "Diffuse Intensity (RGB)");
		Widget::vec3Input(
			"##Inspector_Light_Diffuse",
			light->getDiffuse(),
			[&light](glm::vec3 newDiffuse)
			{
				light->setDiffuse(newDiffuse);
			}
		);

		ImGui::TextColored(Colour::TITLE, "Specular Intensity (RGB)");
		Widget::vec3Input(
			"##Inspector_Light_Specular",
			light->getSpecular(),
			[&light](glm::vec3 newSpecular)
			{
				light->setSpecular(newSpecular);
			}
		);

		// Draw Light subclass sections
		if (DirLight *dir = dynamic_cast<DirLight *>(light))
		{
			drawDirLightSection(dir);
		}
	}


	void _Inspector::drawDirLightSection(DirLight *dir)
	{
		ImGui::TextColored(Colour::TITLE, "Light Direction");
		Widget::vec3Input(
			"##Inspector_DirLight_Direction",
			dir->getDirection(),
			[&dir](glm::vec3 newDirection)
			{
				dir->setDirection(newDirection);
			}
		);
	}


	void _Inspector::drawSpriteSection(Sprite *sprite)
	{
		ImGui::TextColored(Colour::TITLE, "Sprite Texture");
		std::string texPath = sprite->getTexPath().string();
		Widget::textInput(
			"##Inspector_Sprite_Texture",
			texPath,
			[sprite](const std::string &modified)
			{
				if (fs::path{ modified } == sprite->getTexPath()) return;
				sprite->setTexPath(modified);
			},
			texPath
		);

		ImGui::SameLine();
		if (ImGui::SmallButton("..."))
		{
			std::string name = "Load Texture File";
			if (!m_parent->getChild(name))
			{
				auto fileDialog = std::unique_ptr<_FileDialog>(
					new _FileDialog(
						name,
						ROOT_DIRECTORY,
						fs::path(texPath).parent_path(),
						_FileDialogTarget::File,
						[this, sprite](const fs::path &path)
						{
							// Only update the texture if user has selected a valid file
							if (!path.has_filename()) return;
							sprite->setTexPath(path);
						}
					));

				m_parent->addChild(std::move(fileDialog));
			}
		}
	}


	void _Inspector::drawModelSection(Model *model)
	{
		ImGui::TextColored(Colour::TITLE, "Model File");
		std::string modelPath = model->getModelPath().string();
		Widget::textInput(
			"##Inspector_Model_File",
			modelPath,
			[model](const std::string &modified)
			{
				if (fs::path{ modified } == model->getModelPath()) return;
				model->setModelPath(modified);
			},
			modelPath
		);

		ImGui::SameLine();
		if (ImGui::SmallButton("..."))
		{
			std::string name = "Load Model File";
			if (!m_parent->getChild(name))
			{
				auto fileDialog = std::unique_ptr<_FileDialog>(
					new _FileDialog(
						name,
						ROOT_DIRECTORY,
						fs::path(modelPath).parent_path(),
						_FileDialogTarget::File,
						[this, model](const fs::path &path)
						{
							// Only update the model if user has selected a valid file
							if (!path.has_filename()) return;
							model->setModelPath(path);
						}
					));

				m_parent->addChild(std::move(fileDialog));
			}
		}
	}


	/// <summary>
	/// Recurse over all descendants of node, and if any match to the inspected
	/// node, set the inspected node to nullptr (to reflect the deletion).
	/// </summary>
	void _Inspector::onNodeDeleted(Node *node)
	{
		node->forEachDescendant(
			[this](Node *node)
			{
				if (node == m_inspectedNode)
					m_inspectedNode = nullptr;
			},
			[this]()
			{
				return m_inspectedNode == nullptr;
			}
		);
	}


	/// <summary>
	/// Sets the inspected node for the inspector.
	/// </summary>
	void _Inspector::setInspectedNode(Node *node)
	{
		if (m_inspectedNode)
		{
			// Disable outline for prev inspected node (if necessary)
			if (IOutlined *inspectedOutline = dynamic_cast<IOutlined*>(m_inspectedNode))
			{
				inspectedOutline->setOutlineEnabled(false);
			}
		}

		// Enable outline for new inspected node (if necessary)
		if (IOutlined *ioutlined = dynamic_cast<IOutlined*>(node))
		{
			ioutlined->setOutlineEnabled(true);
		}

		m_inspectedNode = node;
	}
}