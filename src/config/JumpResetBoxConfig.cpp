#include "JumpResetBoxConfig.h"

namespace project_diamond
{
	const char* JumpResetBoxConfig::getName() const
	{
		return "JumpResetBox";
	}

	int JumpResetBoxConfig::getJumpResetCount() const
	{
		return m_jumpResetCount;
	}

	void JumpResetBoxConfig::setJumpResetCount(int resetCount)
	{
		m_jumpResetCount = resetCount;
	}
}