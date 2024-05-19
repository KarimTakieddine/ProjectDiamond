#pragma once

#include <BehaviourComponent.h>
#include <MaterialRenderComponent.h>

namespace project_diamond
{
	class ScrollingBackground : public diamond_engine::BehaviourComponent
	{
	public:
		~ScrollingBackground() final override = default;

		const char* getName() const final override;
		void update(GLfloat deltaTime) final override;
		diamond_engine::EngineStatus initialize(const diamond_engine::BehaviourComponentConfig* config) final override;

		void setSpeed(const glm::vec2& speed);

	private:
		glm::vec2 m_speed{ 0.0f, 0.0f };
		diamond_engine::MaterialRenderComponent* m_material{ nullptr };
	};
}
