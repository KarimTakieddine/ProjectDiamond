#pragma once

#include <memory>

#include <diamond_engine/behaviour/Behaviour.h>

namespace diamond_engine { class BehaviourConfig; }

class CharacterController2DBehaviourBuilder
{
public:
	static std::unique_ptr<diamond_engine::Behaviour> Build(const diamond_engine::BehaviourConfig* behaviourConfig);
};
