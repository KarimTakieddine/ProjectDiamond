#include <pugixml.hpp>

#include "Character2DConfigParser.h"

namespace project_diamond
{
	std::unique_ptr<diamond_engine::BehaviourComponentConfig> Character2DConfigParser::parse(const pugi::xml_node& behaviourNode, diamond_engine::EngineStatus* outStatus /* = nullptr */)
	{
		std::unique_ptr<Character2DConfig> result = std::make_unique<Character2DConfig>();

		pugi::xml_attribute gravityAttribute = behaviourNode.attribute("gravity");
		if (gravityAttribute)
			result->setGravity(gravityAttribute.as_float(result->getGravity()));

		pugi::xml_attribute xVelocityAttribute = behaviourNode.attribute("xVelocity");
		if (xVelocityAttribute)
			result->setXVelocity(xVelocityAttribute.as_float(result->getXVelocity()));

		pugi::xml_attribute jumpVelocityAttribute = behaviourNode.attribute("jumpVelocity");
		if (jumpVelocityAttribute)
			result->setJumpVelocity(jumpVelocityAttribute.as_float(result->getJumpVelocity()));

		return result;
	}
}