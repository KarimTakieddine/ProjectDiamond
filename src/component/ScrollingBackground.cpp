#include "GameInstance.h"
#include "MaterialRenderComponent.h"
#include "ScrollingBackground.h"
#include "ScrollingBackgroundConfig.h"

namespace project_diamond
{
	const char* ScrollingBackground::getName() const
	{
		return "ScrollingBackground";
	}

	void ScrollingBackground::update(GLfloat deltaTime)
	{
		m_material->getTextureOffset() += m_speed * deltaTime;
	}

	diamond_engine::EngineStatus ScrollingBackground::initialize(const diamond_engine::BehaviourComponentConfig* config)
	{
		m_material = m_gameInstance->getRenderComponent<diamond_engine::MaterialRenderComponent>("Material");

		if (!m_material)
			return { "ScrollingBackground::initialize failed. No MaterialRenderComponent was found", true };

		const ScrollingBackgroundConfig* scrollingBackgroundConfig = dynamic_cast<const ScrollingBackgroundConfig*>(config);

		if (!scrollingBackgroundConfig)
			return { "ScrollingBackground::initialize failed. Could not convert BehaviourComponentConfig to ScrollingBackgroundConfig", true};

		setSpeed(scrollingBackgroundConfig->getSpeed());

		return { };
	}

	void ScrollingBackground::setSpeed(const glm::vec2& speed)
	{
		m_speed = speed;
	}
}