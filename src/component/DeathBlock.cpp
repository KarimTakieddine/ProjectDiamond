#include <component/Collider2DComponent.h>
#include <component/TransformRenderComponent.h>
#include <engine/GameEngine.h>
#include <game/GameInstance.h>
#include <game/LevelLoader.h>

#include "DeathBlock.h"
#include "DeathBlockConfig.h"

namespace project_diamond
{
	const char* DeathBlock::getName() const
	{
		return "DeathBlock";
	}

	void DeathBlock::update(GLfloat deltaTime)
	{

	}

	diamond_engine::EngineStatus DeathBlock::initialize(const diamond_engine::BehaviourComponentConfig* config)
	{
		const auto* deathBlockConfig = dynamic_cast<const DeathBlockConfig*>(config);

		if (!deathBlockConfig)
		{
			return { "DeathBlock::initialize failed. Could not convert config to target type", true };
		}

		m_resetPosition = deathBlockConfig->getResetPosition();

		return { };
	}

	void DeathBlock::onCollisionEnter2D(const glm::vec2& resolution, diamond_engine::GameInstance* gameInstance, diamond_engine::Collider2DComponent* collider2D)
	{
		if (collider2D->getType() != diamond_engine::ColliderType::CHARACTER)
		{
			return;
		}

		// gameInstance->getRenderComponent<diamond_engine::TransformRenderComponent>("Transform")->setPosition(m_resetPosition);

		auto* gameEngine = m_gameInstance->getGameEngine();

		const auto* currentLevel = diamond_engine::LevelLoader::getInstance().getLevel(gameEngine->getCurrentScene());

		if (!currentLevel)
		{
			// TODO: Log error

			return;
		}

		gameInstance->setActive(false);
		m_gameInstance->getGameEngine()->loadScene(currentLevel);
	}

	const glm::vec3& DeathBlock::getResetPosition() const
	{
		return m_resetPosition;
	}

	void DeathBlock::setResetPosition(const glm::vec3 resetPosition)
	{
		m_resetPosition = resetPosition;
	}
}