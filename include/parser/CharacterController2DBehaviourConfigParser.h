#pragma once

#include <memory>

#include <diamond_engine/config/BehaviourConfig.h>

namespace pugi { class xml_node; }

class CharacterController2DBehaviourConfigParser
{
public:
	static std::unique_ptr<diamond_engine::BehaviourConfig> Parse(const pugi::xml_node& behaviourNode);
};
