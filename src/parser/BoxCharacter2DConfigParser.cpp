#include <pugixml.hpp>

#include "BoxCharacter2DConfig.h"
#include "BoxCharacter2DConfigParser.h"

namespace
{
	using project_diamond::MoveConfig;
	MoveConfig parseMoveConfig(const pugi::xml_node& node)
	{
		MoveConfig result{ };

		pugi::xml_attribute maxVelocityAttribute = node.attribute("maxVelocity");
		if (maxVelocityAttribute)
		{
			result.maxVelocity = maxVelocityAttribute.as_float();
		}

		pugi::xml_attribute accelerationAttribute = node.attribute("acceleration");
		if (accelerationAttribute)
		{
			result.acceleration = accelerationAttribute.as_float();
		}

		pugi::xml_attribute deccelerationAttribute = node.attribute("decceleration");
		if (deccelerationAttribute)
		{
			result.decceleration = deccelerationAttribute.as_float();
		}

		pugi::xml_attribute turnaroundAttribute = node.attribute("turnaround");
		if (turnaroundAttribute)
		{
			result.turnaround = turnaroundAttribute.as_float();
		}

		return result;
	}

	using project_diamond::JumpConfig;
	JumpConfig parseJumpConfig(const pugi::xml_node& node)
	{
		JumpConfig result;

		pugi::xml_attribute gravityAttribute = node.attribute("gravity");
		if (gravityAttribute)
		{
			result.gravity = gravityAttribute.as_float();
		}

		pugi::xml_attribute maxJumpHeightAttribute = node.attribute("maxJumpHeight");
		if (maxJumpHeightAttribute)
		{
			result.maxJumpHeight = maxJumpHeightAttribute.as_float();
		}

		pugi::xml_attribute timeToJumpHeightAttrribute = node.attribute("timeToJumpHeight");
		if (timeToJumpHeightAttrribute)
		{
			result.timeToJumpHeight = timeToJumpHeightAttrribute.as_float();
		}

		pugi::xml_attribute jumpGravityReductionAttribute = node.attribute("jumpGravityReduction");
		if (jumpGravityReductionAttribute)
		{
			result.jumpGravityReduction = jumpGravityReductionAttribute.as_float();
		}

		pugi::xml_attribute maxJumpCounterAttribute = node.attribute("maxJumpCounter");
		if (maxJumpCounterAttribute)
		{
			result.maxJumpCounter = maxJumpCounterAttribute.as_uint();
		}

		return result;
	}
}

namespace project_diamond
{
	/* static */ std::unique_ptr<diamond_engine::BehaviourComponentConfig> BoxCharacter2DConfigParser::parse(
		const pugi::xml_node& behaviourNode,
		diamond_engine::EngineStatus* outStatus /* = nullptr */)
	{
		std::unique_ptr<BoxCharacter2DConfig> config = std::make_unique<BoxCharacter2DConfig>();

		pugi::xml_node walkConfigNode = behaviourNode.child("Walk");
		if (walkConfigNode)
		{
			config->setWalkConfig(::parseMoveConfig(walkConfigNode));
		}

		pugi::xml_node airConfigNode = behaviourNode.child("Air");
		if (airConfigNode)
		{
			config->setAirConfig(::parseMoveConfig(airConfigNode));
		}

		pugi::xml_node jumpConfigNode = behaviourNode.child("Jump");
		if (jumpConfigNode)
		{
			config->setJumpConfig(::parseJumpConfig(jumpConfigNode));
		}

		return config;
	}
}