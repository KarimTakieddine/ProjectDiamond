#pragma once

#include <GL/glew.h>

#include <BehaviourComponentConfig.h>

namespace project_diamond
{
	class Character2DConfig : public diamond_engine::BehaviourComponentConfig
	{
	public:
		virtual ~Character2DConfig() override = default;
		virtual const char* getName() const override;

		void setGravity(GLfloat gravity);
		GLfloat getGravity() const;

		void setJumpVelocity(GLfloat jumpVelocity);
		GLfloat getJumpVelocity() const;

		void setXVelocity(GLfloat xVelocity);
		GLfloat getXVelocity() const;

	private:
		GLfloat m_gravity		{ 0.0f };
		GLfloat m_jumpVelocity	{ 0.0f };
		GLfloat m_xVelocity		{ 0.0f };
	};
}
