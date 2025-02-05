#include <memory>
#include <string>
#include <filesystem>
#include <imgui.h>
#include <glm/gtx/euler_angles.hpp>
#include "transform.hpp"
#include "camera.hpp"
#include "colours.hpp"
#include "file.hpp"
#include "widget.hpp"
#include "nodes/scene.hpp"
#include "nodes/model.hpp"
#include "nodes/inspector.hpp"
#include "nodes/light.hpp"
#include "nodes/camera.hpp"
#include "nodes/ui/ui.hpp"
#include "nodes/ui/console.hpp"


namespace Tank
{
	Inspector::Inspector(const std::string &name) : Panel(name)
	{
		m_inspectedNode = nullptr;
	}


	void Inspector::drawPanel()
	{
		if (m_inspectedNode)
		{
			ImGui::TextColored(Colour::TITLE, "Type");
			ImGui::Text(typeid(*m_inspectedNode).name());

			drawNodeSection();

			// Draw Node subclass sections
			if (Scene *scene = dynamic_cast<Scene *>(m_inspectedNode))
				drawSceneSection(scene);

			if (Model *model = dynamic_cast<Model *>(m_inspectedNode))
				drawModelSection(model);

			if (Camera *camera = dynamic_cast<Camera *>(m_inspectedNode))
				drawCameraSection(camera);

			if (Light *light = dynamic_cast<Light *>(m_inspectedNode))
				drawLightSection(light);
		}	
	}

	
	/// <summary>
	/// Draws inspector section that is present for all Nodes.
	/// </summary>
	void Inspector::drawNodeSection()
	{
		Transform *transform = m_inspectedNode->getTransform();
		const glm::mat4 &modelMatrix = transform->getWorldModelMatrix();

		bool enabled = m_inspectedNode->getEnabled();
		if (ImGui::Checkbox("Enabled", &enabled))
			m_inspectedNode->setEnabled(enabled);

		bool visible = m_inspectedNode->getVisibility();
		if (ImGui::Checkbox("Visible", &visible))
			m_inspectedNode->setVisibility(visible);

		ImGui::TextColored(Colour::TITLE, "Name");
		Widget::textInput("##Inspector_Name", m_inspectedNode->getName(),
			[this](const std::string &newName)
			{
				m_inspectedNode->setName(newName);
			}
		);

		ImGui::TextColored(Colour::TITLE, "Scripts");
		size_t scriptCount = m_inspectedNode->getScriptCount();
		for (int i = 0; i < scriptCount; i++)
		{
			ImGui::Text(typeid(m_inspectedNode->getScript(i)).name());
		}
		if (scriptCount == 0)
		{
			ImGui::Text("None");
		}

		if (ImGui::Button("<Snap To>"))
		{
			auto cam = Tank::Scene::getActiveScene()->getActiveCamera();
			if (cam != nullptr)
			{
				glm::mat4 worldMatrix = transform->getWorldModelMatrix();
				cam->setPosition(Mat4::getTranslation(worldMatrix));
				cam->setRotation(Mat4::getRotation(worldMatrix));
			}
		}

		ImGui::TextColored(Colour::TITLE, "Model Matrix");
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
				transform->setLocalRotation(Quat::fromAngleAxis(newRotation));
			}
		);
	}


	/// <summary>
	/// Draws a section specifically for the root node.
	/// </summary>
	void Inspector::drawSceneSection(Scene *scene)
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
		if (ImGui::SmallButton("Set##INSPECTOR_SCENE_SET_CAM") || m_setPropertyValuePanelActive)
		{
			m_setPropertyValuePanelActive = true;

			ImGui::OpenPopup("##INSPECTOR_SCENE_SET_CAM_LIST");
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
								m_setPropertyValuePanelActive = false;
							}
						}
					}
				);

				ImGui::EndPopup();
			}
		}
	}


	/// <summary>
	/// Draws inspector section that is present for all Models.
	/// </summary>
	void Inspector::drawModelSection(Model *model)
	{
		std::filesystem::path fragPath = model->m_shader->getFragPath();
		std::filesystem::path vertPath = model->m_shader->getVertPath();

		ImGui::TextColored(Colour::TITLE, "Vertex Shader");
		Widget::textInput("##Inspector_VertexShaderPath", vertPath.string(),
			[&model, &fragPath](const std::string &newPath)
			{
				model->setShader(std::make_unique<Shader>(newPath, fragPath.string()));
			}
		);

		std::string vshader;
		if (File::readAllLines("shaders" / vertPath, &vshader))
		{
			// https://github.com/ocornut/imgui/issues/2429
			ImGui::TextUnformatted(vshader.c_str());
		}

		ImGui::TextColored(Colour::TITLE, "Fragment Shader");
		Widget::textInput("##Inspector_FragmentShaderPath", fragPath.string(),
			[&model, &vertPath](const std::string &newPath)
			{
				model->setShader(std::make_unique<Shader>(vertPath.string(), newPath));
			}
		);

		std::string fshader;
		if (File::readAllLines("shaders" / fragPath, &fshader))
		{
			// https://github.com/ocornut/imgui/issues/2429
			ImGui::TextUnformatted(fshader.c_str());
		}
	}


	/// <summary>
	/// Draws inspector section that is present for all Cameras.
	/// </summary>
	void Inspector::drawCameraSection(Camera *camera)
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
		float panSpd = camera->getPanSpeed();
		if (ImGui::InputFloat("##Inspector_Camera_PanSpd", &panSpd))
		{
			camera->setPanSpeed(panSpd);
		}

		ImGui::TextColored(Colour::TITLE, "Camera Rotation Speed");
		float rotSpd = camera->getRotSpeed();
		if (ImGui::InputFloat("##Inspector_Camera_RotSpd", &rotSpd))
		{
			camera->setRotSpeed(rotSpd);
		}
	}


	void Inspector::drawLightSection(Light *light)
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
		//ImGui::Text(glm::to_string(light->getAmbient()).c_str());

		ImGui::TextColored(Colour::TITLE, "Diffuse Intensity (RGB)");
		Widget::vec3Input(
			"##Inspector_Light_Diffuse",
			light->getDiffuse(),
			[&light](glm::vec3 newDiffuse)
			{
				light->setDiffuse(newDiffuse);
			}
		);
		//ImGui::Text(glm::to_string(light->getDiffuse()).c_str());

		ImGui::TextColored(Colour::TITLE, "Specular Intensity (RGB)");
		Widget::vec3Input(
			"##Inspector_Light_Specular",
			light->getSpecular(),
			[&light](glm::vec3 newSpecular)
			{
				light->setSpecular(newSpecular);
			}
		);
		//ImGui::Text(glm::to_string(light->getSpecular()).c_str());

		// Draw Light subclass sections
		if (DirLight *dir = dynamic_cast<DirLight *>(light))
		{
			drawDirLightSection(dir);
		}
	}


	void Inspector::drawDirLightSection(DirLight *dir)
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


	/// <summary>
	/// Recurse over all descendants of node, and if any match to the inspected
	/// node, set the inspected node to nullptr (to reflect the deletion).
	/// </summary>
	void Inspector::onNodeDeleted(Node *node)
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
}