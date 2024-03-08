#include <pugixml.hpp>

#include "CharacterController2DBehaviourConfig.h"
#include "CharacterController2DBehaviourConfigParser.h"

std::unique_ptr<diamond_engine::BehaviourConfig> CharacterController2DBehaviourConfigParser::Parse(const pugi::xml_node& behaviourNode)
{
	return std::make_unique<CharacterController2DBehaviourConfig>();
}
