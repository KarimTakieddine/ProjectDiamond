#pragma once

#include <BehaviourComponentConfig.h>

namespace project_diamond
{
	class Character2DConfig : public diamond_engine::BehaviourComponentConfig
	{
	public:
		~Character2DConfig() final override = default;
		const char* getName() const final override;
	};
}
