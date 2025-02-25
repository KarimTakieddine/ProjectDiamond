#pragma once

#include <glm/vec3.hpp>

#include <component/BehaviourComponent.h>

namespace project_diamond
{
	class DeathBlock : public diamond_engine::BehaviourComponent
	{
	public:
		~DeathBlock() override = default;

		const char* getName() const final override;
		void update(GLfloat deltaTime) final override;
		diamond_engine::EngineStatus initialize(const diamond_engine::BehaviourComponentConfig* config) final override;

		void onCollisionEnter2D(const glm::vec2& resolution, diamond_engine::GameInstance* gameInstance, diamond_engine::Collider2DComponent* collider2D) final override;

		const glm::vec3& getResetPosition() const;
		void setResetPosition(const glm::vec3 resetPosition);

	private:
		glm::vec3 m_resetPosition{ 0.0f, 0.0f, 0.0f };
	};
}
