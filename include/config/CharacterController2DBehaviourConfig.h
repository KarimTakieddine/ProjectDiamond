#pragma once

#include <diamond_engine/config/BehaviourConfig.h>

class CharacterController2DBehaviourConfig : public diamond_engine::BehaviourConfig
{
public:
	const char* GetName() const override;
};