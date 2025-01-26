#pragma once

#include <GL/glew.h>

#include <BehaviourComponentConfig.h>

#include "MoveConfig.h"
#include "JumpConfig.h"

namespace project_diamond
{
	class BoxCharacter2DConfig : public diamond_engine::BehaviourComponentConfig
	{
	public:
		~BoxCharacter2DConfig() override = default;
		const char* getName() const override { return "BoxCharacter2D"; }

		void setWalkConfig(const MoveConfig& config);
		const MoveConfig& getWalkConfig() const;
		MoveConfig& getWalkConfig();

		void setAirConfig(const MoveConfig& config);
		const MoveConfig& getAirConfig() const;
		MoveConfig& getAirConfig();

		void setJumpConfig(const JumpConfig& config);
		const JumpConfig& getJumpConfig() const;
		JumpConfig& getJumpConfig();

	private:
		MoveConfig m_airConfig{ };
		MoveConfig m_walkConfig{ };
		JumpConfig m_jumpConfig{ };
	};
}
