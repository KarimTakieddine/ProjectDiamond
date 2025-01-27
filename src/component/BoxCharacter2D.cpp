#include <audio/AudioEngine.h>
#include <component/Collider2DComponent.h>
#include <game/GameInstance.h>
#include <input/Input.h>

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

		const BoxCharacter2DConfig* boxCharacter2DConfig = dynamic_cast<const BoxCharacter2DConfig*>(config);
		if (!boxCharacter2DConfig)
			return { "Failed to configure BoxCharacter2D instance. Could not convert to target type", true };

		m_walkConfig	= boxCharacter2DConfig->getWalkConfig();
		m_airConfig		= boxCharacter2DConfig->getAirConfig();
		m_jumpConfig	= boxCharacter2DConfig->getJumpConfig();

		m_moveData.accLerp = 0.5f;

		return { };
	}

	void BoxCharacter2D::update(GLfloat deltaTime)
	{
		if (diamond_engine::input::StateMonitor::GetInstance().IsButtonPressed("A"))
		{
			m_jumpData.gravityReduction = glm::clamp(m_jumpConfig.jumpGravityReduction, 0.0f, 1.0f);
		}
		else
		{
			m_jumpData.gravityReduction = 0.0f;
		}

		const float gravityReductionFactor = ( 1.0f - m_jumpData.gravityReduction );

		if ( ( m_collisionState & CollisionState2D::GROUND ) == CollisionState2D::NONE )
		{
			m_jumpData.jumpTimer += deltaTime * gravityReductionFactor;
		}

		m_jumpData.gravity = m_jumpConfig.gravity * gravityReductionFactor;

		if ( ( m_collisionState & CollisionState2D::GROUND ) == CollisionState2D::GROUND )
		{
			m_moveData.config = m_walkConfig;
		}
		else
		{
			m_moveData.config = m_airConfig;
		}

		if ( ( ( m_collisionState & CollisionState2D::GROUND ) == CollisionState2D::GROUND ) || m_jumpData.jumpCounter > 0 )
		{
			if ( diamond_engine::input::StateMonitor::GetInstance().IsButtonDown("A") )
			{
				m_jumpData.jumpTimer			= 0.0f;
				m_jumpData.initialJumpVelocity	= ( ( 2 * m_jumpConfig.maxJumpHeight ) / m_jumpConfig.timeToJumpHeight );

				--( m_jumpData.jumpCounter );
			}
		}

		m_jumpData.velocity = ( -m_jumpData.gravity * m_jumpData.jumpTimer ) + m_jumpData.initialJumpVelocity;

		const float inputX = diamond_engine::input::StateMonitor::GetInstance().getJoystickInput("LeftStick").x;

		if (inputX < 0.0f)
		{
			float reverseForce = 0.0f;

			if ( ( m_moveData.state & MovementState2D::RIGHT ) == MovementState2D::RIGHT )
			{
				if (m_moveData.accLerp > 1.0f)
				{
					m_moveData.accLerp = 1.0f;
				}

				reverseForce = m_moveData.config.turnaround;

				if (m_moveData.accLerp < 0.5f)
				{
					m_moveData.state &= ~MovementState2D::RIGHT;
				}
			}
			else
			{
				reverseForce = m_moveData.config.acceleration;
			}

			m_moveData.state |= MovementState2D::LEFT;

			m_moveData.accLerp -= reverseForce * deltaTime;
			m_moveData.decLerp = 0.0f;
		}
		else if (inputX > 0.0f)
		{
			float reverseForce = 0.0f;

			if ( ( m_moveData.state & MovementState2D::LEFT ) == MovementState2D::LEFT )
			{
				if (m_moveData.accLerp < 0.0f)
				{
					m_moveData.accLerp = 0.0f;
				}

				reverseForce = m_moveData.config.turnaround;

				if (m_moveData.accLerp > 0.5f)
				{
					m_moveData.state &= ~MovementState2D::LEFT;
				}
			}
			else
			{
				reverseForce = m_moveData.config.acceleration;
			}

			m_moveData.state |= MovementState2D::RIGHT;

			m_moveData.accLerp += reverseForce * deltaTime;
			m_moveData.decLerp = 0.0f;
		}
		else if ( ( m_collisionState & CollisionState2D::GROUND ) == CollisionState2D::GROUND )
		{
			m_moveData.decLerp += m_moveData.config.decceleration * deltaTime;
			m_moveData.accLerp = glm::mix(m_moveData.accLerp, 0.5f, glm::clamp(m_moveData.decLerp, 0.0f, 1.0f));
		}

		m_moveData.velocity	= m_moveData.config.maxVelocity *
			glm::mix(-1.0f, 1.0f, glm::clamp(m_moveData.accLerp, 0.0f, 1.0f));

		m_transform->translate(glm::vec2{ m_moveData.velocity, m_jumpData.velocity } * deltaTime);
	}

	void BoxCharacter2D::onCollisionEnter2D(const glm::vec2& resolutionAxis, diamond_engine::GameInstance* gameInstance, diamond_engine::Collider2DComponent* collider2D)
	{
		const std::string& instanceName = collider2D->getGameInstance()->getInternalName();

		if (m_collisionResolutionMap.find(instanceName) != m_collisionResolutionMap.cend())
		{
			return;
		}

		if (resolutionAxis.y > 0.0f)
		{
			m_jumpData.velocity				= 0.0f;
			m_jumpData.jumpTimer			= 0.0f;
			m_jumpData.initialJumpVelocity	= 0.0f;

			m_collisionState |= CollisionState2D::GROUND;

			m_jumpData.jumpCounter = m_jumpConfig.maxJumpCounter;
		}
		else if (resolutionAxis.y < 0.0f)
		{
			m_jumpData.velocity				= 0.0f;
			m_jumpData.initialJumpVelocity	= 0.0f;
		}

		m_collisionResolutionMap.insert({ instanceName, resolutionAxis });
	}

	void BoxCharacter2D::onCollisionExit2D(diamond_engine::GameInstance* gameInstance, diamond_engine::Collider2DComponent* collider2D)
	{
		const std::string& instanceName = collider2D->getGameInstance()->getInternalName();

		auto it = m_collisionResolutionMap.find(instanceName);
		if (it == m_collisionResolutionMap.cend())
		{
			return;
		}

		if (it->second.y > 0.0f)
		{
			m_collisionState &= ~CollisionState2D::GROUND;
		}

		m_collisionResolutionMap.erase(instanceName);
	}

	void BoxCharacter2D::setMaxJumpCount(unsigned int maxJumpCount)
	{
		m_jumpConfig.maxJumpCounter = maxJumpCount;
	}
}