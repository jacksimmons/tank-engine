#pragma once
#define PROJECT_FILE "project.tank"
#include <nodes/interfaces/serialisable.hpp>
#include <assets/resource.hpp>


namespace Tank
{
    class Scene;

    namespace Reflect
    {
        class NodeFactory;
    }


    /// @brief Project in memory.
    class TANK_API Project : public ISerialisable<>
    {
    private:
        /// @brief The last scene loaded in editor.
        Res m_loadedScene;

        Project() {};
    public:
        /// @brief Loads a project from a directory. Uses the project file to do so.
        static std::unique_ptr<Project> loadFromDir(const fs::path &);

        /// @brief Serialises project into json, doesn't save it.
        json serialise();
        /// @brief Deserialises json into a project, and loads it.
        void deserialise(const json &);

        virtual ~Project() {};

        const Res &getSceneRes() const;
    };
}