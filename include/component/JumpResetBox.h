#pragma once

#include <component/BehaviourComponent.h>

namespace diamond_engine
{
	class MaterialRenderComponent;
	class Collider2DComponent;
}

namespace project_diamond
{
	
	class JumpResetBox : public diamond_engine::BehaviourComponent
	{
	public:
		enum class State : unsigned char
		{
			INACTIVE	= 0,
			ACTIVE		= 1
		};

		virtual ~JumpResetBox() override = default;

		const char* getName() const final override;
		void update(GLfloat deltaTime) final override;
		diamond_engine::EngineStatus initialize(const diamond_engine::BehaviourComponentConfig* config) final override;

		void onCollisionEnter2D(const glm::vec2& resolution, diamond_engine::GameInstance* gameInstance, diamond_engine::Collider2DComponent* collider2D) final override;

	private:
		diamond_engine::MaterialRenderComponent* m_materialRenderComponent{ nullptr };
		int m_jumpResetCount{ 0 };
		State m_state{ State::ACTIVE };
	};
}
