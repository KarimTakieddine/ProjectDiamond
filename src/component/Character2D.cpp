#include <AudioEngine.h>
#include <GameInstance.h>
#include <Input.h>

#include "Character2D.h"
#include "Character2DConfig.h"

namespace project_diamond
{
	const char* Character2D::getName() const
	{
		return "Character2D";
	}

	diamond_engine::EngineStatus Character2D::initialize(const diamond_engine::BehaviourComponentConfig* config)
	{
		m_transform = m_gameInstance->getRenderComponent<diamond_engine::TransformRenderComponent>("Transform");

		if (!m_transform)
			return { "Character2D::initialize failed. No game instance transform was found", true };

		m_animationPlayer = m_gameInstance->getBehaviourComponent<diamond_engine::SpriteAnimationPlayer>("SpriteAnimationPlayer");

		const Character2DConfig* characterConfig = dynamic_cast<const Character2DConfig*>(config);
		if (!characterConfig)
			return { "Failed to configure Character2D instance. Could not convert to target type", true };

		m_restingState.gravity		= characterConfig->getGravity();
		m_restingState.jumpVelocity = characterConfig->getJumpVelocity();
		m_restingState.xVelocity	= characterConfig->getXVelocity();

		m_dynamicState.gravity = m_restingState.gravity;

		return { };
	}

	void Character2D::update(GLfloat deltaTime)
	{
		/*if (diamond_engine::input::StateMonitor::GetInstance().IsButtonDown("X"))
		{
			if (m_animationPlayer)
				m_animationPlayer->playAnimation("duck_peck_right");
		}*/

		m_velocity.x = m_restingState.xVelocity * diamond_engine::input::StateMonitor::GetInstance().getJoystickInput("LeftStick").x;

		if (diamond_engine::input::StateMonitor::GetInstance().IsButtonDown("A"))
		{
			if ((m_state & CharacterState::COLLIDING_WITH_GROUND) == CharacterState::COLLIDING_WITH_GROUND)
			{
				m_state |= CharacterState::JUMPING;

				m_dynamicState.gravity		= m_restingState.gravity;
				m_dynamicState.jumpVelocity = m_restingState.jumpVelocity;
				m_jumpTimer					= 0.0f;

				diamond_engine::AudioEngine::getInstance()->playSound2D("jump_character");
			}
		}

		if ((m_state & CharacterState::COLLIDING_WITH_LEFT_WALL) == CharacterState::COLLIDING_WITH_LEFT_WALL)
		{
			if (m_velocity.x < 0.0f)
			{
				m_velocity.x = 0.0f;
			}
		}

		if ((m_state & CharacterState::COLLIDING_WITH_RIGHT_WALL) == CharacterState::COLLIDING_WITH_RIGHT_WALL)
		{
			if (m_velocity.x > 0.0f)
			{
				m_velocity.x = 0.0f;
			}
		}

		m_velocity.y = m_jumpTimer * m_dynamicState.gravity + m_dynamicState.jumpVelocity;

		m_transform->translate(m_velocity * deltaTime);

		if ((m_state & CharacterState::JUMPING) == CharacterState::JUMPING)
		{
			m_jumpTimer += deltaTime;
		}
	}

	void Character2D::onCollisionEnter2D(const glm::vec2& resolutionAxis, const std::string& name)
	{
		if (resolutionAxis.y > 0.0f)
		{
			m_state |= CharacterState::COLLIDING_WITH_GROUND;
			m_state &= ~CharacterState::JUMPING;

			m_dynamicState.gravity		= 0.0f;
			m_dynamicState.jumpVelocity = 0.0f;
			m_jumpTimer					= 0.0f;
		}
		else if (resolutionAxis.y > 0.0f)
		{
			m_state |= CharacterState::COLLDING_WITH_CEILING;
		}
		else if (resolutionAxis.x < 0.0f)
		{
			m_state |= CharacterState::COLLIDING_WITH_RIGHT_WALL;
		}
		else if (resolutionAxis.x > 0.0f)
		{
			m_state |= CharacterState::COLLIDING_WITH_LEFT_WALL;
		}

		m_collisionResolutionMap.insert({ name, resolutionAxis });
	}

	void Character2D::onCollisionExit2D(const std::string& name)
	{
		const auto axisIt = m_collisionResolutionMap.find(name);

		if (axisIt == m_collisionResolutionMap.cend())
		{
			return;
		}

		const glm::vec2& axis = axisIt->second;

		if (axis.y > 0.0f)
		{
			m_state &= ~CharacterState::COLLIDING_WITH_GROUND;

			if ((m_state & CharacterState::JUMPING) == CharacterState::NONE)
			{
				m_dynamicState.jumpVelocity	= 0.0f;
				m_jumpTimer					= 0.0f;
				m_dynamicState.gravity		= m_restingState.gravity;

				m_state |= CharacterState::JUMPING;
			}
		}
		else if (axis.y < 0.0f)
		{
			m_state &= ~CharacterState::COLLDING_WITH_CEILING;
		}
		else if (axis.x < 0)
		{
			m_state &= ~CharacterState::COLLIDING_WITH_RIGHT_WALL;
		}
		else if (axis.x > 0)
		{
			m_state &= ~CharacterState::COLLIDING_WITH_LEFT_WALL;
		}

		m_collisionResolutionMap.erase(name);
	}
}