#pragma once

#include <GL/glew.h>

#include <BehaviourComponentConfig.h>

namespace project_diamond
{
	class BoxCharacter2DConfig : public diamond_engine::BehaviourComponentConfig
	{
	public:
		~BoxCharacter2DConfig() override = default;
		const char* getName() const override { return "BoxCharacter2D"; }
	};
}
