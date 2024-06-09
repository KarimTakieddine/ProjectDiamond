#include "Input.h"
#include "DuckCharacter.h"

namespace project_diamond
{
	const char* DuckCharacter::getName() const
	{
		return "DuckCharacter";
	}

	void DuckCharacter::update(GLfloat deltaTime)
	{
		Character2D::update(deltaTime);

		updateMovementState();

		if (!m_animationPlayer)
			return;

		if (((m_current & MovementState::LEFT) == MovementState::LEFT) && m_previous != MovementState::LEFT)
		{
			m_animationPlayer->playAnimation("duck_left", true);
			m_previous = MovementState::LEFT;
		}
		else if ((m_current & MovementState::RIGHT) == MovementState::RIGHT && m_previous != MovementState::RIGHT)
		{
			m_animationPlayer->playAnimation("duck_right", true);
			m_previous = MovementState::RIGHT;
		}

		if (diamond_engine::input::StateMonitor::GetInstance().IsButtonDown("X"))
		{
			m_animationPlayer->playAnimation(getPeckAnimationName(), true);
		}
	}

	void DuckCharacter::updateMovementState()
	{
		if (m_velocity.x < 0.0f)
		{
			m_current |= MovementState::LEFT;
			m_current &= ~(MovementState::RIGHT | MovementState::NEUTRAL);
		}
		else if (m_velocity.x > 0.0f)
		{
			m_current |= MovementState::RIGHT;
			m_current &= ~(MovementState::LEFT | MovementState::NEUTRAL);
		}
		else
		{
			m_current = MovementState::NEUTRAL;
		}
	}

	std::string DuckCharacter::getPeckAnimationName() const
	{
		if ((m_current & MovementState::RIGHT) == MovementState::RIGHT)
		{
			return "duck_peck_right";
		}
		else if ((m_current & MovementState::LEFT) == MovementState::LEFT)
		{
			return "duck_peck_left";
		}
		else if ((m_previous & MovementState::LEFT) == MovementState::LEFT)
		{
			return "duck_peck_left";
		}
		else if ((m_previous & MovementState::RIGHT) == MovementState::RIGHT)
		{
			return "duck_peck_right";
		}
		else
		{
			return "duck_peck_right";
		}
	}
}