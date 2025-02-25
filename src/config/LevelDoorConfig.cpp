#include "LevelDoorConfig.h"

namespace project_diamond
{
	const char* LevelDoorConfig::getName() const
	{
		return "LevelDoor";
	}

	const std::string& LevelDoorConfig::getNextLevel() const
	{
		return m_nextLevel;
	}

	void LevelDoorConfig::setNextLevel(const std::string& name)
	{
		m_nextLevel = name;
	}
}