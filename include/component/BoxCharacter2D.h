#pragma once

#include <type_traits>
#include <unordered_map>

#include <glm/vec2.hpp>

#include <component/BehaviourComponent.h>
#include <component/TransformRenderComponent.h>
#include <config/BehaviourComponentConfig.h>

#include "CollisionState2D.h"
#include "JumpConfig.h"
#include "MoveConfig.h"
#include "MovementState2D.h"

namespace project_diamond
{
	class BoxCharacter2D : public diamond_engine::BehaviourComponent
	{
	public:
		struct MoveData
		{
			MoveConfig config{ };
			MovementState2D state{ MovementState2D::NONE };
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

		CollisionState2D m_collisionState{ CollisionState2D::NONE };
	};
}
