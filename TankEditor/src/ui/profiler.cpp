#include <nodes/scene.hpp>
#include <nodes/interfaces/mesh_container.hpp>
#include <nodes/interfaces/shader_container.hpp>
#include <texture.hpp>
#include <utils/flip.hpp>
#include "profiler.hpp"
#include "static/time.hpp"
#include "colours.hpp"


namespace Tank
{
	template <class T>
	static void ifSubclass(Node *node, std::function<void(T*)> then)
	{
		// If the node can be casted to this node subtype, then draw using the subtype inspector.
		if (T *t = dynamic_cast<T *>(node))
		{
			then(t);
		}
	}


	static void addNodeMem(std::unordered_map<std::string, int> &memMap, Node *node, const std::string &keyPrefix = "")
	{
		memMap[keyPrefix + "Node"] += sizeof(*node);
		memMap[keyPrefix + "Node"] += sizeof(Transform);

		ifSubclass<IMeshContainer>(node, [&node, &memMap, &keyPrefix](IMeshContainer *mesh)
		{
			for (const auto *mesh : mesh->getMeshes())
			{
				// Meshes
				const auto &vertices = mesh->getVertices();
				const auto &indices = mesh->getIndices();
				memMap[keyPrefix + "Mesh"] += vertices.size() * sizeof(Vertex);
				memMap[keyPrefix + "Mesh"] += indices.size() * sizeof(unsigned);
			}
		});
	}
}


namespace Tank::Editor
{
	const WindowOpts WINDOW_OPTS = {
		ImGuiWindowFlags_None,
		true,
		false
	};


	_Profiler::_Profiler(const std::string &name)
		: _Window(name, WINDOW_OPTS)
	{
	}


	void _Profiler::drawPanel()
	{
		ImGui::TextColored(Colour::TITLE, "Frame Delta");
		{
			ImGui::Text(std::format("{} (FPS: {})", Time::FrameDelta(), 1 / Time::FrameDelta()).c_str());
		}

		ImGui::TextColored(Colour::TITLE, "Memory");
		{
			std::unordered_map<std::string, int> memoryMap;

			Node *editor = getParent();
			Scene *scene = Scene::getActiveScene();

			// Count memory usage of editor-only nodes
			for (Node *child : editor->getChildrenOfType<Node>())
			{
				if (child != scene)
				{
					addNodeMem(memoryMap, child, "Editor:");
				}
			}

			// Count memory usage of textures
			for (const auto &texture : Texture::getLoadedTextures())
			{
				auto tex = texture.get();
				memoryMap["Texture"] += sizeof(tex);
				memoryMap["Texture"] += tex->Width() * tex->Height() * tex->NumChannels() * tex->Depth();
			}

			addNodeMem(memoryMap, scene);
			scene->forEachDescendant([&memoryMap](Node *node)
			{
				addNodeMem(memoryMap, node);
			});

			auto flipped = Utils::flipMap(memoryMap);
			for (auto it = flipped.begin(); it != flipped.end(); ++it)
			{
				ImGui::Text(std::format("{}: {} KB", it->second, it->first / 1000.0f).c_str());
			}
		}
	}
}