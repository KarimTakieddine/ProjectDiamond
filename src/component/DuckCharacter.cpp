#include <Collider2DComponent.h>
#include <EngineMacros.h>
#include <GameInstanceManager.h>
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

	void DuckCharacter::initializeCollider()
	{
		if (m_collider)
			return;

		auto gameInstance = diamond_engine::GameInstanceManager::getInstance()->findGameInstance("duck_character_collider");

		if (!gameInstance)
			return;

		m_collider = gameInstance->getBehaviourComponent<diamond_engine::Collider2DComponent>("Collider2D");

		if (!m_collider)
			return;

		m_colliderOffset = m_collider->getOffset();
	}

	void DuckCharacter::update(GLfloat deltaTime)
	{
		initializeCollider();

		Character2D::update(deltaTime);

		updateMovementState();

		if (!m_animationPlayer)
			return;

		if (((m_current & MovementState::LEFT) == MovementState::LEFT) && m_previous != MovementState::LEFT)
		{
			m_animationPlayer->playAnimation("duck_sprite_left", true);
			m_previous = MovementState::LEFT;

			if (m_collider)
				m_collider->setOffset({ -m_colliderOffset.x, m_colliderOffset.y });

			DEBUG_EXEC(diamond_engine::Debugger::getInstance()->debugEvent(
				diamond_engine::DebugEvent::Type::SPRITE_ANIMATION_PLAY,
				std::make_unique<diamond_engine::SpriteAnimationPlay>("duck left", true)));
		}
		else if ((m_current & MovementState::RIGHT) == MovementState::RIGHT && m_previous != MovementState::RIGHT)
		{
			m_animationPlayer->playAnimation("duck_sprite_right", true);
			m_previous = MovementState::RIGHT;

			if (m_collider)
				m_collider->setOffset({ m_colliderOffset.x, m_colliderOffset.y });

			DEBUG_EXEC(diamond_engine::Debugger::getInstance()->debugEvent(
				diamond_engine::DebugEvent::Type::SPRITE_ANIMATION_PLAY,
				std::make_unique<diamond_engine::SpriteAnimationPlay>("duck right", true)));
		}

		if (diamond_engine::input::StateMonitor::GetInstance().IsButtonDown("Y"))
		{
			const std::string wingStrikeAnimationName = getWingStrikeAnimationName();
			DEBUG_EXEC(
				if (wingStrikeAnimationName == "duck_sprite_wing_strike_right")
				{
					DEBUG_EXEC(diamond_engine::Debugger::getInstance()->debugEvent(
						diamond_engine::DebugEvent::Type::SPRITE_ANIMATION_PLAY,
						std::make_unique<diamond_engine::SpriteAnimationPlay>("duck wing right", true)));
				}

				if (wingStrikeAnimationName == "duck_sprite_wing_strike_left")
				{
					DEBUG_EXEC(diamond_engine::Debugger::getInstance()->debugEvent(
						diamond_engine::DebugEvent::Type::SPRITE_ANIMATION_PLAY,
						std::make_unique<diamond_engine::SpriteAnimationPlay>("duck wing left", true)));
				}
			);

			m_animationPlayer->playAnimation(wingStrikeAnimationName, true);
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

	std::string DuckCharacter::getWingStrikeAnimationName() const
	{
		if ((m_current & MovementState::RIGHT) == MovementState::RIGHT)
		{
			return "duck_sprite_wing_strike_right";
		}
		else if ((m_current & MovementState::LEFT) == MovementState::LEFT)
		{
			return "duck_sprite_wing_strike_right";
		}
		else if ((m_previous & MovementState::LEFT) == MovementState::LEFT)
		{
			return "duck_sprite_wing_strike_right";
		}
		else if ((m_previous & MovementState::RIGHT) == MovementState::RIGHT)
		{
			return "duck_sprite_wing_strike_right";
		}
		else
		{
			return "duck_sprite_wing_strike_right";
		}
	}
}