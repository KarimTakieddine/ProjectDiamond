#include <component/MaterialRenderComponent.h>
#include <game/GameInstance.h>

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

	void JumpResetBox::onCollisionEnter2D(const glm::vec2& resolution, const std::string& name)
	{
		if (m_state == State::INACTIVE)
		{
			return;
		}
		
		if (resolution.y < 0.0f)
		{
			m_materialRenderComponent->setColor(glm::vec3(0.0f, 0.0f, 1.0f));

			m_state = State::INACTIVE;
		}
	}
}