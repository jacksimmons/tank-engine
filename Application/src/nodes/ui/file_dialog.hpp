#pragma once
#include "window.hpp"
namespace fs = std::filesystem;


namespace Tank::Editor
{
	typedef std::function<void(const fs::path &)> _FileDialogCallback;
	enum class _FileDialogTarget { Directory, File };
	class _FileDialog final : public _Window
	{
		// Editor can instantiate these - Load/Save scene
		friend class EditorApp;

	private:
		struct _DirectoryResult
		{
			fs::path path;
			fs::perms perms;
		};

		struct _FileResult
		{
			fs::path path;
			std::string name;
			fs::perms perms;
		};

		/// <summary>
		/// Starting directory; user may not exit this directory.
		/// </summary>
		fs::path m_rootDirectory;
		/// <summary>
		/// The directory the user starts at (a subdirectory of m_rootDirectory)
		/// </summary>
		fs::path m_startDirectory;
		
		_FileDialogTarget m_target;
		_FileDialogCallback m_onTargetSelected;
		fs::path m_targetSelected;

		/// <summary>
		/// The current navigated-to directory (a subdirectory of m_rootDirectory)
		/// </summary>
		fs::path m_currentDirectory;

		std::vector<_DirectoryResult> m_currentSubdirectories;
		std::vector<_FileResult> m_currentDirectoryFiles;

		_FileDialog(const std::string &name,
			const fs::path &rootDirectory,
			const fs::path &startDirectory,
			_FileDialogTarget target,
			_FileDialogCallback onTargetSelected
		);

		/// <summary>
		/// Updates m_currentSubdirectories and m_currentDirectoryFiles.
		/// </summary>
		void updateDirectoryItems();
		void drawTargetView();
		void drawDirectoryFiles();
		void drawSubdirectories();
		void drawTargetSelectBar();
	protected:
		virtual void drawPanel() override;
	public:
		~_FileDialog() = default;
	};
}