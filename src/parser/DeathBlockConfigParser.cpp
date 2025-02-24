#include <pugixml.hpp>

#include <parser/Vector3Parser.h>

#include "DeathBlockConfig.h"
#include "DeathBlockConfigParser.h"

namespace project_diamond
{
    /* static */std::unique_ptr<diamond_engine::BehaviourComponentConfig> DeathBlockConfigParser::parse(const pugi::xml_node& behaviourNode, diamond_engine::EngineStatus* outStatus)
    {
        std::unique_ptr<DeathBlockConfig> config = std::make_unique<DeathBlockConfig>();

        pugi::xml_node resetPositionChild = behaviourNode.child("ResetPosition");

        if (resetPositionChild)
        {
            config->setResetPosition(diamond_engine::Vector3Parser::Parse(resetPositionChild));
        }

        return config;
    }
}