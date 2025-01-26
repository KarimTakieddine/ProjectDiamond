#pragma once

#include <type_traits>
#include <unordered_map>

#include <glm/vec2.hpp>

#include <BehaviourComponent.h>
#include <BehaviourComponentConfig.h>
#include <TransformRenderComponent.h>

#include "JumpConfig.h"
#include "MoveConfig.h"

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
		struct MoveData
		{
			MoveConfig config{ };
			CharacterMovementState state{ CharacterMovementState::NONE };
			float velocity;
			float accLerp;
			float decLerp;
		};

		struct JumpData
		{
			float velocity				{ 0.0f };
			float gravity				{ 0.0f };
			float jumpTimer				{ 0.0f };
			float gravityReduction		{ 0.0f };
			float initialJumpVelocity	{ 0.0f };
			unsigned int jumpCounter	{ 0 };
		};

		virtual ~BoxCharacter2D() override = default;

		virtual const char* getName() const override;
		virtual void update(GLfloat deltaTime) override;
		diamond_engine::EngineStatus initialize(const diamond_engine::BehaviourComponentConfig* config) final override;

		void onCollisionEnter2D(const glm::vec2& resolutionAxis, const std::string& name) final override;
		void onCollisionExit2D(const std::string& name) final override;

	private:
		std::unordered_map<std::string, glm::vec2> m_collisionResolutionMap;
		diamond_engine::TransformRenderComponent* m_transform{ nullptr };
		MoveConfig m_walkConfig	{ };
		MoveConfig m_airConfig	{ };
		JumpConfig m_jumpConfig	{ };
		MoveData m_moveData{ };
		JumpData m_jumpData{ };

		CollisionState m_collisionState{ CollisionState::NONE };
	};
}
