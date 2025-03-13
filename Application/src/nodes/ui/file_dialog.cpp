#include <imgui.h>
#include "log.hpp"
#include "file_dialog.hpp"
#include "colours.hpp"
#include "widget.hpp"
namespace fs = std::filesystem;


namespace Tank::Editor
{
	_FileDialog::_FileDialog(const std::string &name,
		const std::filesystem::path &rootDirectory,
		const std::filesystem::path &startDirectory,
		_FileDialogTarget target,
		_FileDialogCallback onTargetSelected
	) : _Window(name),
		m_rootDirectory(rootDirectory), m_startDirectory(startDirectory), m_currentDirectory(startDirectory), m_target(target),
		m_onTargetSelected(onTargetSelected)
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

		ImGui::BeginChild("##FILE_DIALOG_VIEW", viewSize);
		drawTargetView();
		ImGui::EndChild();

		drawTargetSelectBar();
	}


	void _FileDialog::drawTargetView()
	{
		// Setup table
		if (!ImGui::BeginTable("##FILE_DIALOG_TABLE", 2))
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
		drawDirectoryFiles();

		ImGui::EndTable();
	}


	void _FileDialog::updateDirectoryItems()
	{
		m_currentSubdirectories.clear();
		m_currentDirectoryFiles.clear();
		
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
				// If directory is the target type, select when clicked once.
				if (m_target == _FileDialogTarget::Directory)
					m_targetSelected = subdir.path;

				// The directory is entered either way.
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
					m_targetSelected = file.path;
			}

			// Type
			ImGui::TableNextColumn();
			ImGui::Text("File");

			ImGui::PopStyleColor();
		}
	}


	void _FileDialog::drawTargetSelectBar()
	{
		ImGui::Text(m_targetSelected.string().c_str());
		ImGui::SameLine();

		// If a target is selected, show "Select" button which will close the dialog when pressed.
		if (!m_targetSelected.empty() && ImGui::SmallButton("Select##FILE_DIALOG_TARGET_SELECT"))
		{
			m_onTargetSelected(m_targetSelected);
		}
	}
}