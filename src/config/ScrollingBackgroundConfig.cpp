#include "ScrollingBackgroundConfig.h"

namespace project_diamond
{
	const char* ScrollingBackgroundConfig::getName() const
	{
		return "ScrollingBackground";
	}

	void ScrollingBackgroundConfig::setSpeed(const glm::vec2& speed)
	{
		m_speed = speed;
	}

	const glm::vec2& ScrollingBackgroundConfig::getSpeed() const
	{
		return m_speed;
	}
}