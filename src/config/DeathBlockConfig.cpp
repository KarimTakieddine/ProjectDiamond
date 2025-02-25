#include "DeathBlockConfig.h"

namespace project_diamond
{
	const char* DeathBlockConfig::getName() const
	{
		return "DeathBlock";
	}

	const glm::vec3& DeathBlockConfig::getResetPosition() const
	{
		return m_resetPosition;
	}

	void DeathBlockConfig::setResetPosition(const glm::vec3& resetPosition)
	{
		m_resetPosition = resetPosition;
	}
}