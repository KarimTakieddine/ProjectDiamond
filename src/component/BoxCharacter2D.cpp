#include <AudioEngine.h>
#include <GameInstance.h>
#include <Input.h>

#include "BoxCharacter2D.h"
#include "BoxCharacter2DConfig.h"

#include <iostream>

namespace project_diamond
{
	const char* BoxCharacter2D::getName() const
	{
		return "BoxCharacter2D";
	}

	diamond_engine::EngineStatus BoxCharacter2D::initialize(const diamond_engine::BehaviourComponentConfig* config)
	{
		m_transform = m_gameInstance->getRenderComponent<diamond_engine::TransformRenderComponent>("Transform");

		if (!m_transform)
			return { "BoxCharacter2D::initialize failed. No game instance transform was found", true };

		const BoxCharacter2DConfig* characterConfig = dynamic_cast<const BoxCharacter2DConfig*>(config);
		if (!characterConfig)
			return { "Failed to configure BoxCharacter2D instance. Could not convert to target type", true };

		return { };
	}

	void BoxCharacter2D::update(GLfloat deltaTime)
	{
		if (diamond_engine::input::StateMonitor::GetInstance().IsButtonPressed("A"))
		{
			m_jumpGravityReduction = 0.10f;
		}
		else
		{
			m_jumpGravityReduction = 0.0f;
		}

		if ( ( m_collisionState & CollisionState::GROUND ) == CollisionState::NONE )
		{
			m_jumpTimer += deltaTime * (1.0f - m_jumpGravityReduction);
		}

		m_gravity = 9.8f * (1.0f - m_jumpGravityReduction);

		if ( ( ( m_collisionState & CollisionState::GROUND ) == CollisionState::GROUND ) || m_jumpCounter > 0 )
		{
			m_accelerationForce		= 1.0f;
			m_deccelerationForce	= 2.0f;
			m_turnaroundForce		= 3.0f;

			if ( diamond_engine::input::StateMonitor::GetInstance().IsButtonDown("A") )
			{
				m_jumpTimer				= 0.0f;
				m_initialJumpVelocity	= ( ( 2 * m_jumpHeight ) / m_timeToJumpHeight );

				--m_jumpCounter;
			}
		}
		else
		{
			m_accelerationForce		= 0.5f;
			m_deccelerationForce	= 1.0f;
			m_turnaroundForce		= 1.5f;
		}

		m_velocityY = ( -m_gravity * m_jumpTimer ) + m_initialJumpVelocity;

		const float inputX = diamond_engine::input::StateMonitor::GetInstance().getJoystickInput("LeftStick").x;

		if (inputX < 0.0f)
		{
			float reverseForce = 0.0f;

			if ( ( m_movementState & CharacterMovementState::RIGHT ) == CharacterMovementState::RIGHT )
			{
				if (m_accelerationInterpolantX > 1.0f)
				{
					m_accelerationInterpolantX = 1.0f;
				}

				reverseForce = m_turnaroundForce;

				if (m_accelerationInterpolantX < 0.5f)
				{
					m_movementState &= ~CharacterMovementState::RIGHT;
				}
			}
			else
			{
				reverseForce = m_accelerationForce;
			}

			m_movementState |= CharacterMovementState::LEFT;

			m_accelerationInterpolantX -= reverseForce * deltaTime;
			m_deccelerationInterpolantX = 0.0f;
		}
		else if (inputX > 0.0f)
		{
			float reverseForce = 0.0f;

			if ( ( m_movementState & CharacterMovementState::LEFT ) == CharacterMovementState::LEFT )
			{
				if (m_accelerationInterpolantX < 0.0f)
				{
					m_accelerationInterpolantX = 0.0f;
				}

				reverseForce = m_turnaroundForce;

				if (m_accelerationInterpolantX > 0.5f)
				{
					m_movementState &= ~CharacterMovementState::LEFT;
				}
			}
			else
			{
				reverseForce = m_accelerationForce;
			}

			m_movementState |= CharacterMovementState::RIGHT;

			m_accelerationInterpolantX += reverseForce * deltaTime;
			m_deccelerationInterpolantX = 0.0f;
		}
		else
		{
			m_deccelerationInterpolantX += m_deccelerationForce * deltaTime;

			m_accelerationInterpolantX = glm::mix(m_accelerationInterpolantX, 0.5f, glm::clamp(m_deccelerationInterpolantX, 0.0f, 1.0f));
		}

		m_accelerationX = glm::mix(-1.0f, 1.0f, glm::clamp(m_accelerationInterpolantX, 0.0f, 1.0f));
		m_velocityX		= m_maxVelocityX * m_accelerationX;
		
		m_transform->translate(glm::vec2{ m_velocityX, m_velocityY } * deltaTime);

		/*m_velocity.x = m_restingState.xVelocity * diamond_engine::input::StateMonitor::GetInstance().getJoystickInput("LeftStick").x;

		if (diamond_engine::input::StateMonitor::GetInstance().IsButtonDown("A"))
		{
			if ((m_state & CharacterState::COLLIDING_WITH_GROUND) == CharacterState::COLLIDING_WITH_GROUND)
			{
				m_state |= CharacterState::JUMPING;

				m_dynamicState.gravity = m_restingState.gravity;
				m_dynamicState.jumpVelocity = m_restingState.jumpVelocity;
				m_jumpTimer = 0.0f;

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
		}*/
	}

	void BoxCharacter2D::onCollisionEnter2D(const glm::vec2& resolutionAxis, const std::string& name)
	{
		if (m_collisionResolutionMap.find(name) != m_collisionResolutionMap.cend())
		{
			return;
		}

		if (resolutionAxis.y > 0.0f)
		{
			m_velocityY				= 0.0f;
			m_jumpTimer				= 0.0f;
			m_initialJumpVelocity	= 0.0f;

			m_collisionState |= CollisionState::GROUND;

			m_jumpCounter = m_maxJumpCounter;
		}
		else if (resolutionAxis.y < 0.0f)
		{
			m_velocityY		= 0.0f;
			m_jumpTimer		= 0.0f;
		}

		m_collisionResolutionMap.insert({ name, resolutionAxis });
	}

	void BoxCharacter2D::onCollisionExit2D(const std::string& name)
	{
		auto it = m_collisionResolutionMap.find(name);
		if (it == m_collisionResolutionMap.cend())
		{
			return;
		}

		if (it->second.y > 0.0f)
		{
			m_collisionState &= ~CollisionState::GROUND;
		}

		m_collisionResolutionMap.erase(name);
	}
}