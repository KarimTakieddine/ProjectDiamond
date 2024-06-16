#include <EngineMacros.h>
#include <SpriteAnimationPlay.h>

#include "Input.h"
#include "DuckCharacter.h"
#include "Debugger.h"

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

			DEBUG_EXEC(diamond_engine::Debugger::getInstance()->debugEvent(
				diamond_engine::DebugEvent::Type::SPRITE_ANIMATION_PLAY,
				std::make_unique<diamond_engine::SpriteAnimationPlay>("duck left", true)));
		}
		else if ((m_current & MovementState::RIGHT) == MovementState::RIGHT && m_previous != MovementState::RIGHT)
		{
			m_animationPlayer->playAnimation("duck_right", true);
			m_previous = MovementState::RIGHT;

			DEBUG_EXEC(diamond_engine::Debugger::getInstance()->debugEvent(
				diamond_engine::DebugEvent::Type::SPRITE_ANIMATION_PLAY,
				std::make_unique<diamond_engine::SpriteAnimationPlay>("duck right", true)));
		}

		if (diamond_engine::input::StateMonitor::GetInstance().IsButtonDown("Y"))
		{
			const std::string peckAnimationName = getPeckAnimationName();
			DEBUG_EXEC(
				if (peckAnimationName == "duck_peck_right")
				{
					DEBUG_EXEC(diamond_engine::Debugger::getInstance()->debugEvent(
						diamond_engine::DebugEvent::Type::SPRITE_ANIMATION_PLAY,
						std::make_unique<diamond_engine::SpriteAnimationPlay>("duck peck right", true)));
				}

				if (peckAnimationName == "duck_peck_left")
				{
					DEBUG_EXEC(diamond_engine::Debugger::getInstance()->debugEvent(
						diamond_engine::DebugEvent::Type::SPRITE_ANIMATION_PLAY,
						std::make_unique<diamond_engine::SpriteAnimationPlay>("duck peck left", true)));
				}
			);

			m_animationPlayer->playAnimation(peckAnimationName, true);
		}
		else if (diamond_engine::input::StateMonitor::GetInstance().IsButtonDown("X"))
		{
			const std::string quackAnimationName = getQuackAnimationName();

			DEBUG_EXEC(
				if (quackAnimationName == "duck_quack_right")
				{
					DEBUG_EXEC(diamond_engine::Debugger::getInstance()->debugEvent(
						diamond_engine::DebugEvent::Type::SPRITE_ANIMATION_PLAY,
						std::make_unique<diamond_engine::SpriteAnimationPlay>("duck quack right", true)));
				}
			);

			m_animationPlayer->playAnimation(quackAnimationName, true);
			m_animationPlayer->playAnimation("duck_right", false);
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

	std::string DuckCharacter::getQuackAnimationName() const
	{
		if ((m_current & MovementState::RIGHT) == MovementState::RIGHT)
		{
			return "duck_quack_right";
		}
		else
		{
			return "duck_quack_right";
		}
	}
}