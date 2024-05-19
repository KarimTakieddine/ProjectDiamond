#pragma once

#include <memory>

#include <ComponentConfigParser.h>

#include "ScrollingBackgroundConfig.h"

namespace pugi { class xml_node; }

namespace project_diamond
{
	class ScrollingBackgroundConfigParser
	{
	public:
		static std::unique_ptr<diamond_engine::BehaviourComponentConfig> parse(const pugi::xml_node& behaviourNode, diamond_engine::EngineStatus* outStatus = nullptr);
	};
}
