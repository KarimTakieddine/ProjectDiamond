#include <pugixml.hpp>

#include "JumpResetBoxConfig.h"
#include "JumpResetBoxConfigParser.h"

namespace project_diamond
{
    /* static */std::unique_ptr<diamond_engine::BehaviourComponentConfig> JumpResetBoxConfigParser::parse(const pugi::xml_node& behaviourNode, diamond_engine::EngineStatus* outStatus)
    {
        std::unique_ptr<JumpResetBoxConfig> config = std::make_unique<JumpResetBoxConfig>();

        pugi::xml_attribute jumpResetCountAttribute = behaviourNode.attribute("jumpResetCountAttribute");
        if (jumpResetCountAttribute)
            config->setJumpResetCount(jumpResetCountAttribute.as_uint());

        return config;
    }
}