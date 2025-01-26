#include "BoxCharacter2DConfig.h"

namespace project_diamond
{
	void BoxCharacter2DConfig::setWalkConfig(const MoveConfig& config)
	{
		m_walkConfig = config;
	}

	const MoveConfig& BoxCharacter2DConfig::getWalkConfig() const
	{
		return m_walkConfig;
	}

	MoveConfig& BoxCharacter2DConfig::getWalkConfig()
	{
		return m_walkConfig;
	}

	void BoxCharacter2DConfig::setAirConfig(const MoveConfig& config)
	{
		m_airConfig = config;
	}

	const MoveConfig& BoxCharacter2DConfig::getAirConfig() const
	{
		return m_airConfig;
	}

	MoveConfig& BoxCharacter2DConfig::getAirConfig()
	{
		return m_airConfig;
	}

	void BoxCharacter2DConfig::setJumpConfig(const JumpConfig& config)
	{
		m_jumpConfig = config;
	}

	const JumpConfig& BoxCharacter2DConfig::getJumpConfig() const
	{
		return m_jumpConfig;
	}

	JumpConfig& BoxCharacter2DConfig::getJumpConfig()
	{
		return m_jumpConfig;
	}
}