#include <AudioEngine.h>
#include <GameInstance.h>
#include <Input.h>

#include "BoxCharacter2D.h"
#include "BoxCharacter2DConfig.h"

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
		const float inputX = diamond_engine::input::StateMonitor::GetInstance().getJoystickInput("LeftStick").x;

		m_accelerationForce		= 1.0f;
		m_deccelerationForce	= 1.0f;

		if (inputX < 0.0f)
		{
			if (m_movingRight)
			{
				if (m_accelerationInterpolantX > 1.0f)
				{
					m_accelerationInterpolantX = 1.0f;
				}
			}

			m_accelerationInterpolantX -= m_accelerationForce * deltaTime;
			m_deccelerationInterpolantX = 0.0f;
			
			m_movingLeft	= true;
			m_movingRight	= false;
		}
		else if (inputX > 0.0f)
		{
			if (m_movingLeft)
			{
				if (m_accelerationInterpolantX < 0.0f)
				{
					m_accelerationInterpolantX = 0.0f;
				}
			}

			m_accelerationInterpolantX += m_accelerationForce * deltaTime;
			m_deccelerationInterpolantX = 0.0f;

			m_movingLeft	= false;
			m_movingRight	= true;
		}
		else
		{
			m_deccelerationInterpolantX += m_deccelerationForce * deltaTime;

			m_accelerationInterpolantX = glm::mix(m_accelerationInterpolantX, 0.5f, glm::clamp(m_deccelerationInterpolantX, 0.0f, 1.0f));
		}

		m_accelerationX = glm::mix(-1.0f, 1.0f, glm::clamp(m_accelerationInterpolantX, 0.0f, 1.0f));
		m_velocityX = m_maxVelocityX * m_accelerationX;
		
		m_transform->translate(glm::vec2{ m_velocityX, 0.0f } * deltaTime);

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
		m_collisionResolutionMap.insert({ name, resolutionAxis });
	}

	void BoxCharacter2D::onCollisionExit2D(const std::string& name)
	{
		m_collisionResolutionMap.erase(name);
	}
}