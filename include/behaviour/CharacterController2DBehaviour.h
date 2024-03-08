#pragma once

#include <type_traits>
#include <unordered_map>

#include <glm/vec2.hpp>

#include <diamond_engine/behaviour/Behaviour.h>

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

class CharacterController2DBehaviour : public diamond_engine::Behaviour
{
public:
	void Update(GLfloat deltaTime) override;

	void OnCollisionEnter(const glm::vec2& resolutionAxis, const std::string& name) override;
	void OnCollisionExit(const std::string& name) override;

private:
	std::unordered_map<std::string, glm::vec2> m_collisionResolutionMap;
	glm::vec2 m_velocity{ 0.0f, 0.0f };
	GLfloat m_gravity{ -6.5f };
	GLfloat m_initialJumpVelocity{ 0.0f };
	float m_jumpTimer{ 0.0f };
	CharacterState m_state{ CharacterState::JUMPING };
};
