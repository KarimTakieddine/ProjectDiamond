#pragma once

#include "Character2D.h"

namespace diamond_engine { class Collider2DComponent; }

namespace project_diamond
{
	enum class MovementState : GLubyte
	{
		UNKNOWN	= 0,
		NEUTRAL = 1,
		LEFT	= 1 << 1,
		RIGHT	= 1 << 2
	};

	inline MovementState operator|(MovementState a, MovementState b)
	{
		return static_cast<MovementState>(static_cast<std::underlying_type_t<MovementState>>(a) | static_cast<std::underlying_type_t<MovementState>>(b));
	}

	inline MovementState& operator|=(MovementState& a, MovementState b)
	{
		a = (a | b);

		return a;
	}

	inline MovementState operator&(MovementState a, MovementState b)
	{
		return static_cast<MovementState>(static_cast<std::underlying_type_t<MovementState>>(a) & static_cast<std::underlying_type_t<MovementState>>(b));
	}

	inline MovementState& operator&=(MovementState& a, MovementState b)
	{
		a = (a & b);

		return a;
	}

	inline MovementState operator~(MovementState a)
	{
		return static_cast<MovementState>(~(static_cast<std::underlying_type_t<MovementState>>(a)));
	}

	class DuckCharacter : public Character2D
	{
	public:
		~DuckCharacter() final override = default;

		const char* getName() const final override;
		void update(GLfloat deltaTime) final override;

	private:
		void updateMovementState();
		void initializeCollider();
		std::string getPeckAnimationName() const;
		std::string getWingStrikeAnimationName() const;
		std::string getQuackAnimationName() const;
		glm::vec2 m_colliderOffset{ 0.0f, 0.0f };
		diamond_engine::Collider2DComponent* m_collider{ nullptr };
		MovementState m_current	{ MovementState::NEUTRAL };
		MovementState m_previous{ MovementState::NEUTRAL };
	};
}
