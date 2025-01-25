#pragma once

#include <type_traits>
#include <unordered_map>

#include <glm/vec2.hpp>

#include <BehaviourComponent.h>
#include <BehaviourComponentConfig.h>
#include <SpriteAnimationPlayer.h>
#include <TransformRenderComponent.h>

namespace project_diamond
{
	enum class CollisionState : unsigned short
	{
		NONE	= 0,
		GROUND	= 1
	};

	inline CollisionState operator|(CollisionState a, CollisionState b)
	{
		return static_cast<CollisionState>(
			static_cast<std::underlying_type_t<CollisionState>>(a) |
			static_cast<std::underlying_type_t<CollisionState>>(b));
	}

	inline CollisionState& operator|=(CollisionState& a, CollisionState b)
	{
		a = (a | b);

		return a;
	}

	inline CollisionState operator&(CollisionState a, CollisionState b)
	{
		return static_cast<CollisionState>(
			static_cast<std::underlying_type_t<CollisionState>>(a) &
			static_cast<std::underlying_type_t<CollisionState>>(b));
	}

	inline CollisionState& operator&=(CollisionState& a, CollisionState b)
	{
		a = (a & b);

		return a;
	}

	inline CollisionState operator~(CollisionState a)
	{
		return static_cast<CollisionState>(~(static_cast<std::underlying_type_t<CollisionState>>(a)));
	}

	enum class CharacterMovementState : unsigned short
	{
		NONE	= 0,
		LEFT	= 1,
		RIGHT	= 1 << 1
	};

	inline CharacterMovementState operator|(CharacterMovementState a, CharacterMovementState b)
	{
		return static_cast<CharacterMovementState>(
			static_cast<std::underlying_type_t<CharacterMovementState>>(a) |
			static_cast<std::underlying_type_t<CharacterMovementState>>(b));
	}

	inline CharacterMovementState& operator|=(CharacterMovementState& a, CharacterMovementState b)
	{
		a = (a | b);

		return a;
	}

	inline CharacterMovementState operator&(CharacterMovementState a, CharacterMovementState b)
	{
		return static_cast<CharacterMovementState>(
			static_cast<std::underlying_type_t<CharacterMovementState>>(a) &
			static_cast<std::underlying_type_t<CharacterMovementState>>(b));
	}

	inline CharacterMovementState& operator&=(CharacterMovementState& a, CharacterMovementState b)
	{
		a = (a & b);

		return a;
	}

	inline CharacterMovementState operator~(CharacterMovementState a)
	{
		return static_cast<CharacterMovementState>(~(static_cast<std::underlying_type_t<CharacterMovementState>>(a)));
	}

	class BoxCharacter2D : public diamond_engine::BehaviourComponent
	{
	public:
		virtual ~BoxCharacter2D() override = default;

		virtual const char* getName() const override;
		virtual void update(GLfloat deltaTime) override;
		diamond_engine::EngineStatus initialize(const diamond_engine::BehaviourComponentConfig* config) final override;

		void onCollisionEnter2D(const glm::vec2& resolutionAxis, const std::string& name) final override;
		void onCollisionExit2D(const std::string& name) final override;

	private:
		std::unordered_map<std::string, glm::vec2> m_collisionResolutionMap;
		diamond_engine::TransformRenderComponent* m_transform{ nullptr };

		CollisionState m_collisionState{ CollisionState::NONE };
		CharacterMovementState m_movementState{ CharacterMovementState::NONE };

		float m_accelerationX{ 0.0f };
		float m_velocityX{ 0.0f };
		float m_accelerationForce{ 0.0f };
		float m_deccelerationForce{ 0.0f };
		float m_turnaroundForce{ 0.0f };
		float m_maxVelocityX{ 6.5f };
		float m_accelerationInterpolantX{ 0.5f };
		float m_deccelerationInterpolantX{ 0.0f };

		float m_velocityY{ 0.0f };
		float m_gravity{ 9.8f };
		float m_jumpTimer{ 0.0f };
		float m_timeToJumpHeight{ 1.0f };
		float m_jumpHeight{ 5.0f };
		float m_initialJumpVelocity{ 0.0f };
		float m_jumpGravityReduction{ 0.0f };
	};
}
