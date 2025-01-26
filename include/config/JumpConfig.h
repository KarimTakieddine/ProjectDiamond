#pragma once

namespace project_diamond
{
	struct JumpConfig
	{
		float gravity				{ 0.0f };
		float maxJumpHeight			{ 0.0f };
		float timeToJumpHeight		{ 0.0f };
		float jumpGravityReduction	{ 0.0f };
		unsigned int maxJumpCounter	{ 0 };
	};
}
