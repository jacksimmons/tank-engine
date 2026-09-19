#include <nfd.h>
#include <imgui.h>
#include <ui/FileDialog.h>
#include <nodes/Scene.h>
#include <Serialisation.h>
#include <Log.h>
#include <fs/Dir.h>
#include "ProjectsMenu.h"


namespace Tank::Editor
{
    ProjectsMenu::ProjectsMenu(std::function<void(fs::path)> loadProject)
    : UINode("ProjectsMenu"), m_loadProject(loadProject)
    {
    }


    void ProjectsMenu::drawUI()
    {
        if (ImGui::Button("New Project"))
        {
            fs::path demoDir = fs::path{ "demo-project" };
            fs::path projectDir = FileDialog::open(FileDialog::Target::Directory);
            if (projectDir == "") return;
            
            Dir::tryCopy(demoDir, projectDir, fs::copy_options::recursive | fs::copy_options::overwrite_existing);

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
