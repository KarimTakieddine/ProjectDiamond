#include <pugixml.hpp>

#include <Vector2Parser.h>

#include "ScrollingBackgroundConfigParser.h"

namespace project_diamond
{
	std::unique_ptr<diamond_engine::BehaviourComponentConfig> ScrollingBackgroundConfigParser::parse(const pugi::xml_node& behaviourNode, diamond_engine::EngineStatus* outStatus /* = nullptr */)
	{
		std::unique_ptr<ScrollingBackgroundConfig> result = std::make_unique<ScrollingBackgroundConfig>();

		pugi::xml_node speedNode = behaviourNode.child("Speed");

		if (speedNode)
			result->setSpeed(diamond_engine::Vector2Parser::Parse(speedNode));

		return result;
	}
}