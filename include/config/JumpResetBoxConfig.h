#pragma once

#include <config/BehaviourComponentConfig.h>

namespace project_diamond
{
	class JumpResetBoxConfig : public diamond_engine::BehaviourComponentConfig
	{
	public:
		virtual ~JumpResetBoxConfig() override = default;

		const char* getName() const final override;

		int getJumpResetCount() const;
		void setJumpResetCount(int resetCount);

	private:
		int m_jumpResetCount{ 0 };
	};
}
