#include <pugixml.hpp>

#include "LevelDoorConfig.h"
#include "LevelDoorConfigParser.h"

namespace project_diamond
{
    /* static */std::unique_ptr<diamond_engine::BehaviourComponentConfig> LevelDoorConfigParser::parse(const pugi::xml_node& behaviourNode, diamond_engine::EngineStatus* outStatus)
    {
        std::unique_ptr<LevelDoorConfig> config = std::make_unique<LevelDoorConfig>();

        pugi::xml_attribute nextLevelAttribute = behaviourNode.attribute("nextLevel");

        if (!nextLevelAttribute)
        {
            if (outStatus)
            {
                outStatus->message  = "Failed to parse LevelDoor config. No \"nextLevel\" was set";
                outStatus->error    = true;
            }

            return nullptr;
        }

        config->setNextLevel(nextLevelAttribute.as_string());

        return config;
    }
}