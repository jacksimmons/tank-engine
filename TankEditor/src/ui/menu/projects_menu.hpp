#pragma once
#include <events/event.hpp>
#include "menu.hpp"


namespace Tank::Editor
{
    class ProjectsMenu final : public Menu
    {
    private:
        std::function<void(fs::path)> m_loadProject;
    public:
        ProjectsMenu(std::function<void(fs::path)> loadProject);

        void draw() override;
    };
}