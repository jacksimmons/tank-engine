#include <scene_serialisation.hpp>
#include <file.hpp>
#include <log.hpp>
#include "project.hpp"


namespace Tank
{
    json Project::serialise()
    {
        json j;
        j["lastLoadedScene"] = Res::encode(m_loadedScene);
        return j;
    }

    void Project::deserialise(const json &j)
    {
        m_loadedScene = Res::decode(j["lastOpenScene"]);
    }

    std::unique_ptr<Project> Project::loadFromDir(const fs::path &dir)
    {
        fs::path projectFilePath = dir / PROJECT_FILE;
        std::string projectFileLines;
        if (File::readLines(projectFilePath, projectFileLines) == File::ReadResult::Success)
        {
            json serialised;
            try
            {
                serialised = json::parse(projectFileLines);

                auto project = std::unique_ptr<Project>(new Project());
                project->deserialise(serialised);
                return project;
            }
            catch (std::exception e)
            {
                TE_ERROR(std::format("Couldn't parse project file {}.", projectFilePath.string()));
                return nullptr;
            }
        }

        TE_ERROR(std::format("Couldn't read project file {}.", projectFilePath.string()));
        return nullptr;
    }


    const Res &Project::getSceneRes() const
    {
        return m_loadedScene;
    }
}