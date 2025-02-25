#include <component/Collider2DComponent.h>
#include <component/MaterialRenderComponent.h>
#include <game/GameInstance.h>

#include "BoxCharacter2D.h"
#include "JumpResetBox.h"
#include "JumpResetBoxConfig.h"

namespace project_diamond
{
	const char* JumpResetBox::getName() const
	{
		return "JumpResetBox";
	}

	void JumpResetBox::update(GLfloat deltaTime)
	{

	}

	diamond_engine::EngineStatus JumpResetBox::initialize(const diamond_engine::BehaviourComponentConfig* config)
	{
		m_materialRenderComponent = m_gameInstance->getRenderComponent<diamond_engine::MaterialRenderComponent>("Material");
		if (!m_materialRenderComponent)
			return { "Could not find material on JumpResetBox component instance", true };

		const JumpResetBoxConfig* boxConfig = dynamic_cast<const JumpResetBoxConfig*>(config);

		m_jumpResetCount = boxConfig->getJumpResetCount();

		return { };
	}

	void JumpResetBox::onCollisionEnter2D(const glm::vec2& resolution, diamond_engine::GameInstance* gameInstance, diamond_engine::Collider2DComponent* collider2D)
	{
		if (m_state == State::INACTIVE)
		{
			return;
		}

		if (collider2D->getType() != diamond_engine::ColliderType::CHARACTER)
		{
			return;
		}
		
		if (resolution.y < 0.0f)
		{
			auto* boxCharacter2D = gameInstance->getBehaviourComponent<BoxCharacter2D>("BoxCharacter2D");

			if (!boxCharacter2D)
			{
				// TODO: More error handling...
				return;
			}

			boxCharacter2D->setMaxJumpCount(m_jumpResetCount);

			m_materialRenderComponent->setColor(glm::vec3(0.59f, 0.29f, 0.0f));

			m_state = State::INACTIVE;
		}
	}
}