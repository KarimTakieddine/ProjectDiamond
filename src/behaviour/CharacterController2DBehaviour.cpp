#include <diamond_engine/audio/AudioEngine.h>
#include <diamond_engine/gameobject/GameObject.h>
#include <diamond_engine/component/SpriteAnimationPlayer.h>
#include <diamond_engine/input/Input.h>

#include "behaviour/CharacterController2DBehaviour.h"

void CharacterController2DBehaviour::Update(GLfloat deltaTime)
{
	const float horizontalSpeed		= 3.5f;
	const float gravity				= -6.5f;
	const float initialJumpVelocity = 5.0f;

	m_velocity = glm::vec2{ horizontalSpeed, 0.0f } *diamond_engine::input::StateMonitor::GetInstance().getJoystickInput("LeftStick");

	if (diamond_engine::input::StateMonitor::GetInstance().IsButtonDown("A"))
	{
		if ((m_state & CharacterState::COLLIDING_WITH_GROUND) == CharacterState::COLLIDING_WITH_GROUND)
		{
			m_state |= CharacterState::JUMPING;
			m_jumpTimer = 0.0f;
			m_gravity = gravity;
			m_initialJumpVelocity = initialJumpVelocity;

			diamond_engine::SpriteAnimationPlayer* spriteAnimationPlayer = m_gameObject->GetComponent<diamond_engine::SpriteAnimationPlayer>("AnimationPlayer");
			if (spriteAnimationPlayer)
			{
				spriteAnimationPlayer->playAnimation("jumpSpriteSheet");
			}

			diamond_engine::AudioEngine::instance()->playSound2D("jump_character");
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

	m_velocity.y = m_jumpTimer * m_gravity + m_initialJumpVelocity;

	m_gameObject->GetRenderableObject()->transform.Translate(m_velocity * deltaTime);

	if ((m_state & CharacterState::JUMPING) == CharacterState::JUMPING)
	{
		m_jumpTimer += deltaTime;
	}
}

void CharacterController2DBehaviour::OnCollisionEnter(const glm::vec2& resolutionAxis, const std::string& name)
{
	if (resolutionAxis.y > 0.0f)
	{
		m_state |= CharacterState::COLLIDING_WITH_GROUND;
		m_state &= ~CharacterState::JUMPING;

		m_gravity				= 0.0f;
		m_initialJumpVelocity	= 0.0f;
		m_jumpTimer				= 0.0f;
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

void CharacterController2DBehaviour::OnCollisionExit(const std::string& name)
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
			m_initialJumpVelocity = 0.0f;
			m_gravity = -6.5f;
			m_jumpTimer = 0.0f;
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