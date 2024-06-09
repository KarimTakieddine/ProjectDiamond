#include "Character2DConfig.h"
#include "Character2DConfigParser.h"
#include "DuckCharacterConfigParser.h"

namespace project_diamond
{
	/* static */ std::unique_ptr<diamond_engine::BehaviourComponentConfig>
		DuckCharacterConfigParser::parse(const pugi::xml_node& behaviourNode, diamond_engine::EngineStatus* outStatus /* = nullptr */)
	{
		auto characterConfig		= Character2DConfigParser::parse(behaviourNode, outStatus);
		const Character2DConfig* characterConfigData	= dynamic_cast<const Character2DConfig*>(characterConfig.get());

		if (!characterConfigData)
		{
			if (outStatus)
				*outStatus = { "Failed to convert Character2D component as part of DuckCharacter", true };

			return nullptr;
		}

		auto result = std::make_unique<DuckCharacterConfig>();
		result->setXVelocity(characterConfigData->getXVelocity());
		result->setGravity(characterConfigData->getGravity());
		result->setJumpVelocity(characterConfigData->getJumpVelocity());

		return result;
	}
}