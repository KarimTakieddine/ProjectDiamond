#include <stdexcept>

#include "CharacterController2DBehaviour.h"
#include "CharacterController2DBehaviourBuilder.h"
#include "CharacterController2DBehaviourConfig.h"

/* static */ std::unique_ptr<diamond_engine::Behaviour> CharacterController2DBehaviourBuilder::Build(const diamond_engine::BehaviourConfig* behaviourConfig)
{
	const CharacterController2DBehaviourConfig* config = dynamic_cast<const CharacterController2DBehaviourConfig*>(behaviourConfig);

	if (!config)
	{
		throw std::runtime_error("Failed to convert BehaviourConfig to CharacterController2DBehaviourConfig");
	}

	std::unique_ptr<CharacterController2DBehaviour> result = std::make_unique<CharacterController2DBehaviour>();



	return result;
}