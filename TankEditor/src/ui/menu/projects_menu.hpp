#pragma once
#include <nodes/ui/ui_node.hpp>


namespace Tank::Editor
{
    class ProjectsMenu final : public UINode
    {
    private:
        std::function<void(fs::path)> m_loadProject;
    public:
        ProjectsMenu(std::function<void(fs::path)> loadProject);

        void drawUI() override;
    };
}