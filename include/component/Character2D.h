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
	enum class CharacterState : unsigned short
	{
		NONE						= 0,
		COLLIDING_WITH_GROUND		= 1,
		COLLIDING_WITH_LEFT_WALL	= 1 << 1,
		COLLIDING_WITH_RIGHT_WALL	= 1 << 2,
		COLLDING_WITH_CEILING		= 1 << 3,
		JUMPING						= 1 << 4
	};

	inline CharacterState operator|(CharacterState a, CharacterState b)
	{
		return static_cast<CharacterState>(static_cast<std::underlying_type_t<CharacterState>>(a) | static_cast<std::underlying_type_t<CharacterState>>(b));
	}

	inline CharacterState& operator|=(CharacterState& a, CharacterState b)
	{
		a = (a | b);

		return a;
	}

	inline CharacterState operator&(CharacterState a, CharacterState b)
	{
		return static_cast<CharacterState>(static_cast<std::underlying_type_t<CharacterState>>(a) & static_cast<std::underlying_type_t<CharacterState>>(b));
	}

	inline CharacterState& operator&=(CharacterState& a, CharacterState b)
	{
		a = (a & b);

		return a;
	}

	inline CharacterState operator~(CharacterState a)
	{
		return static_cast<CharacterState>(~(static_cast<std::underlying_type_t<CharacterState>>(a)));
	}

	class Character2D : public diamond_engine::BehaviourComponent
	{
	public:
		struct PhysicsState
		{
			GLfloat gravity		{ 0.0f };
			GLfloat jumpVelocity{ 0.0f };
			GLfloat xVelocity	{ 0.0f };
		};

		~Character2D() final override = default;

		const char* getName() const final override;
		void update(GLfloat deltaTime) final override;
		diamond_engine::EngineStatus initialize(const diamond_engine::BehaviourComponentConfig* config) final override;

		void onCollisionEnter2D(const glm::vec2& resolutionAxis, const std::string& name) final override;
		void onCollisionExit2D(const std::string& name) final override;

	private:
		std::unordered_map<std::string, glm::vec2> m_collisionResolutionMap;
		PhysicsState m_restingState{ };
		PhysicsState m_dynamicState{ };
		glm::vec2 m_velocity{ 0.0f, 0.0f };
		diamond_engine::TransformRenderComponent* m_transform{ nullptr };
		diamond_engine::SpriteAnimationPlayer* m_animationPlayer{ nullptr };
		float m_jumpTimer{ 0.0f };
		CharacterState m_state{ CharacterState::JUMPING };
	};
}
