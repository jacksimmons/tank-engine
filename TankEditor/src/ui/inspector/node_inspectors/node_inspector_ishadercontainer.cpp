#include <imgui.h>
#include <colours.hpp>
#include <widget.hpp>
#include <shader_source.hpp>
#include <nodes/interfaces/shader_container.hpp>
#include "node_inspector.hpp"


namespace Tank
{
	/// <summary>
	/// Draws section for a single shader source (e.g. frag shader).
	/// </summary>
	static std::string drawShaderSourceSection(const std::string &sourceName, ShaderSource &source)
	{
		std::string retPath;

		std::string shaderType = sourceName + " shader";
		ImGui::TextColored(Colour::TITLE, shaderType.c_str());
		Widget::textInput(("##Inspector_" + shaderType).c_str(), source.location.string(),
			[&retPath](const std::string &newPath)
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
}


namespace Tank::Editor
{
	/// <summary>
	/// Draws inspector section that is present for all nodes with shaders.
	/// </summary>
	void _NodeInspector<IShaderContainer>::draw()
	{
		const ShaderSources &sources = m_node->getShader().getShaderSources();
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
				m_node->setShader(std::move(maybeShader.value()));
			}
		}
	}
}