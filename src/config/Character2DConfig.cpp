#include "Character2DConfig.h"

namespace project_diamond
{
	const char* Character2DConfig::getName() const
	{
		return "Character2D";
	}

	void Character2DConfig::setGravity(GLfloat gravity)
	{
		m_gravity = gravity;
	}

	GLfloat Character2DConfig::getGravity() const
	{
		return m_gravity;
	}

	void Character2DConfig::setJumpVelocity(GLfloat jumpVelocity)
	{
		m_jumpVelocity = jumpVelocity;
	}

	GLfloat Character2DConfig::getJumpVelocity() const
	{
		return m_jumpVelocity;
	}

	void Character2DConfig::setXVelocity(GLfloat xVelocity)
	{
		m_xVelocity = xVelocity;
	}

	GLfloat Character2DConfig::getXVelocity() const
	{
		return m_xVelocity;
	}
}