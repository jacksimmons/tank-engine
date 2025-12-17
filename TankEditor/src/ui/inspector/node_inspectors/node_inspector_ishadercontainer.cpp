#include <imgui.h>
#include <colours.hpp>
#include <file.hpp>
#include <widget.hpp>
#include <shader.hpp>
#include <nodes/interfaces/shader_container.hpp>
#include <static/time.hpp>
#include "node_inspector.hpp"


namespace Tank
{
	/// <summary>
	/// Draws section for a single shader source (e.g. frag shader).
	/// </summary>
	static std::string drawShaderSourceSection(const std::string &sourceName, const ShaderSource &source, bool readFile,
		std::unordered_map<std::string, std::string> &sourceContents)
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

		ImGui::SameLine();
		if (ImGui::Button("Open in VSCode"))
		{
			system(std::format("code {}", ("shaders" / source.location).string()).c_str());
		}

		// Read the file if contents is not provided. If that read fails...
		if (readFile && File::readLines("shaders" / source.location, sourceContents[sourceName]) != File::ReadResult::Success)
		{
			TE_CORE_TRACE(std::format("Couldn't read shader file {}", source.location.string()));
			sourceContents[sourceName] = "<N/A>";
		}

		// https://github.com/ocornut/imgui/issues/2429
		ImGui::TextUnformatted(sourceContents[sourceName].c_str());

		return retPath;
	}
}


namespace Tank::Editor
{
	/// <summary>
	/// Draws inspector section that is present for all nodes with shaders.
	/// </summary>
	template <>
	void _NodeInspector<IShaderContainer>::draw()
	{
		const ShaderSources &sources = m_node->getShader().getShaderSources();
		ShaderSources copy = ShaderSources(sources);

		static TimePoint previousTime;
		TimePoint currentTime = Time::CurrentTime();
		static std::unordered_map<std::string, std::string> previousContents;
		bool readFile = false;
		if (std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - previousTime).count() > 1000)
		{
			readFile = true;
			previousTime = currentTime;
		}

		std::string vertLoc = drawShaderSourceSection("Vertex", copy.vertex, readFile, previousContents);
		if (!vertLoc.empty())
		{
			copy.vertex.location = vertLoc;
		}

		std::string fragLoc = drawShaderSourceSection("Fragment", copy.fragment, readFile, previousContents);
		if (!fragLoc.empty())
		{
			copy.fragment.location = fragLoc;
		}

		std::string geomLoc = drawShaderSourceSection("Geometry", copy.geometry, readFile, previousContents);
		if (!geomLoc.empty())
		{
			copy.geometry.location = geomLoc;
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