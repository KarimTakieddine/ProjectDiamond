#pragma once

#include <string>

#include "component/BehaviourComponent.h"

namespace project_diamond
{
	class LevelDoor : public diamond_engine::BehaviourComponent
	{
	public:
		const char* getName() const final override;
		void update(GLfloat deltaTime) final override;
		diamond_engine::EngineStatus initialize(const diamond_engine::BehaviourComponentConfig* config) final override;

		void onCollisionEnter2D(
			const glm::vec2& resolution,
			diamond_engine::GameInstance* gameInstance,
			diamond_engine::Collider2DComponent* collider2D) final override;

		void onCollisionExit2D(
			diamond_engine::GameInstance* gameInstance,
			diamond_engine::Collider2DComponent* collider2D) final override;

	private:
		std::string m_nextLevel;
		bool m_characterPresent{ false };
	};
}
