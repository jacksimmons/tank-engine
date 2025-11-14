#include <imgui.h>
#include "log.hpp"
#include "file_dialog.hpp"
#include "colours.hpp"
#include "widget.hpp"
namespace fs = std::filesystem;


namespace Tank::Editor
{
	const WindowOpts WINDOW_OPTS = {
		ImGuiWindowFlags_None,
		true,
		false
	};


	_FileDialog::_FileDialog(const std::string &name,
		const std::filesystem::path &rootDirectory,
		const std::filesystem::path &startDirectory,
		_FileDialogTarget target,
		_FileDialogCallback onTargetSelected
	) : _Window(name, WINDOW_OPTS),
		m_rootDirectory(rootDirectory), m_startDirectory(startDirectory), m_currentDirectory(startDirectory), m_target(target),
		m_onTargetSelected(onTargetSelected), m_searchTerm("")
	{
		// If current directory doesn't contain the root directory
		if (m_startDirectory.string().find(m_rootDirectory.string()) == std::string::npos)
		{
			TE_CORE_CRITICAL("Invalid FileDialog: Current directory must be a subdirectory of the root directory.");
		}
	}


	void _FileDialog::drawPanel()
	{
		ImVec2 viewSize = ImGui::GetWindowSize();
		viewSize.y -= 60;

		if (ImGui::BeginChild(std::string{ getName() + "##FILE_DIALOG_VIEW" }.c_str(), viewSize))
		{
			drawTargetView();
			drawTargetBar();
		}
		ImGui::EndChild();
	}


	void _FileDialog::drawTargetView()
	{
		// Setup table
		if (!ImGui::BeginTable(std::string{ getName() + "##FILE_DIALOG_TABLE" }.c_str(), 2))
		{
			return;
		}

		ImGui::TableSetupColumn("Name");
		ImGui::TableSetupColumn("Type");

		// Allow going back a directory when user wants to, unless already at root.
		ImGui::TableHeadersRow();
		if (m_currentDirectory != m_rootDirectory)
		{
			ImGui::TableNextColumn();
			if (ImGui::Button("../"))
			{
				m_currentDirectory = m_currentDirectory.parent_path();
				TE_CORE_INFO(m_currentDirectory.string());
				ImGui::EndTable();
				return;
			}
			ImGui::TableNextColumn();
		}

		updateDirectoryItems();

		// Determine if the current directory is accessible
		// This is important when m_currentDirectory == m_startDirectory
		// Return now if not accessible
		if (!((fs::status(m_currentDirectory).permissions() & fs::perms::owner_read) == fs::perms::owner_read))
		{
			ImGui::TextColored(Colour::ERR, "ACCESS DENIED");
			ImGui::EndTable();
			return;
		}

		drawSubdirectories();
		if (m_target == _FileDialogTarget::File) drawDirectoryFiles();

		ImGui::EndTable();
	}


	void _FileDialog::updateDirectoryItems()
	{
		m_currentSubdirectories.clear();
		m_currentDirectoryFiles.clear();
		m_filesMatchingSearchTerm.clear();
		
		std::filesystem::directory_iterator dirIter;

		try
		{
			dirIter = fs::directory_iterator{ m_currentDirectory };
		}
		catch (fs::filesystem_error e)
		{
			// Nothing to update.
			return;
		}

		for (const auto &dirItem : dirIter)
		{
			fs::path path = dirItem.path();

			// Handle file additions
			if (fs::is_regular_file(path))
			{
				_FileResult file;
				file.path = path;
				file.name = path.filename().string();
				// Check if its name contains the search term, if it doesn't then skip
				if (m_searchTerm.length() > 0 && file.name.find(m_searchTerm) == std::string::npos) continue;

				file.perms = fs::status(path).permissions();

				m_currentDirectoryFiles.push_back(file);
			}
			
			// Handle directories
			else if (fs::is_directory(path))
			{
				_DirectoryResult dir;
				dir.path = path;
				dir.perms = fs::status(path).permissions();

				m_currentSubdirectories.push_back(dir);
			}
		}
	}


	void _FileDialog::drawSubdirectories()
	{
		// Draw all subdirectories
		for (const auto &subdir : m_currentSubdirectories)
		{
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(Colour::DIRECTORY));

			// Name
			ImGui::TableNextColumn();
			if (ImGui::Selectable(subdir.path.string().c_str()))
			{
				// Update current directory.
				m_currentDirectory = subdir.path;
			}

			// Type
			ImGui::TableNextColumn();
			ImGui::Text("Directory");

			ImGui::PopStyleColor();
		}
	}


	void _FileDialog::drawDirectoryFiles()
	{
		// Draw all files
		for (const auto &file : m_currentDirectoryFiles)
		{
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(Colour::FILE));

			// Name
			ImGui::TableNextColumn();
			if (ImGui::Selectable(file.path.string().c_str()))
			{
				if (m_target == _FileDialogTarget::File)
					m_currentTarget = file.path;
			}

			// Type
			ImGui::TableNextColumn();
			ImGui::Text("File");

			ImGui::PopStyleColor();
		}
	}


	void _FileDialog::drawTargetBar()
	{
		// If the current target is non-empty
		std::string inputFieldHint;
		if (!getCurrentTarget().empty()) inputFieldHint = getCurrentTarget().string();
		else inputFieldHint = "Enter name...";
		Widget::textInput(std::string{ getName() + "##FILE_DIALOG_SEARCH_TERM" }.c_str(), inputFieldHint.c_str(), [this](const std::string &modified)
		{
			m_searchTerm = modified;
			m_currentTarget = m_currentDirectory / m_searchTerm;
		});

		// If a target is selected, show "Select" button which will close the dialog when pressed.
		ImGui::SameLine();
		if (!getCurrentTarget().empty() && ImGui::SmallButton(std::string{ "Select##FILE_DIALOG_TARGET_SELECT(" + getName() + ")" }.c_str()))
		{
			// Select the current directory as a target, when target is a directory.
			// Otherwise, select the current target.
			m_onTargetSelected(getCurrentTarget());
			destroy();
		}
	}


	fs::path _FileDialog::getCurrentTarget() const
	{
		return m_target == _FileDialogTarget::Directory ? m_currentDirectory : m_currentTarget;
	}
}