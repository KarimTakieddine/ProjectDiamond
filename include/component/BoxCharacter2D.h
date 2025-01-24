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
	/*
	enum class CharacterState : unsigned short
	{
		NONE = 0,
		COLLIDING_WITH_GROUND = 1,
		COLLIDING_WITH_LEFT_WALL = 1 << 1,
		COLLIDING_WITH_RIGHT_WALL = 1 << 2,
		COLLDING_WITH_CEILING = 1 << 3,
		JUMPING = 1 << 4
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
	*/

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

		float m_accelerationX{ 0.0f };
		float m_velocityX{ 0.0f };
		float m_accelerationForce{ 0.0f };
		float m_deccelerationForce{ 0.0f };
		float m_maxVelocityX{ 6.5f };
		float m_accelerationInterpolantX{ 0.5f };
		float m_deccelerationInterpolantX{ 0.0f };
		bool m_movingLeft{ false };
		bool m_movingRight{ false };
	};
}
