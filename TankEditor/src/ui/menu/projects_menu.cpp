#include <nfd.h>
#include <imgui.h>
#include <ui/file_dialog.hpp>
#include <nodes/scene.hpp>
#include <serialisation.hpp>
#include "projects_menu.hpp"


namespace Tank::Editor
{
    ProjectsMenu::ProjectsMenu(std::function<void(fs::path)> loadProject)
    : Menu("ProjectsMenu"), m_loadProject(loadProject)
    {
    }


    void ProjectsMenu::draw()
    {
        if (ImGui::Button("New Project"))
        {
            fs::path projectDir = FileDialog::open(FileDialog::Target::Directory);
            if (projectDir == "") return;
            fs::copy("DemoProject", projectDir, fs::copy_options::recursive);

            m_loadProject(projectDir);
        }

        if (ImGui::Button("Open Project"))
        {
            fs::path projectDir = FileDialog::open(FileDialog::Target::Directory);
            if (projectDir == "") return;

            m_loadProject(projectDir);
        }
    }
}