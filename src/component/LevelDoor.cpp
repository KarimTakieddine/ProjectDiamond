#include <component/Collider2DComponent.h>
#include <engine/EngineStorage.h>
#include <engine/GameEngine.h>
#include <game/GameInstance.h>
#include <game/LevelLoader.h>
#include <input/Input.h>

#include "BoxCharacter2D.h"
#include "LevelDoor.h"
#include "LevelDoorConfig.h"

namespace project_diamond
{
	const char* LevelDoor::getName() const
	{
		return "LevelDoor";
	}

	void LevelDoor::update(GLfloat deltaTime)
	{
		if (m_character && diamond_engine::input::StateMonitor::GetInstance().IsButtonPressed("DPad_up"))
		{
			const auto* nextLevel = diamond_engine::LevelLoader::getInstance().getLevel(m_nextLevel);

			if (!nextLevel)
			{
				// TODO: Log error

				return;
			}

			const GLuint totalJumpCount = m_character->getTotalJumpCount();

			diamond_engine::EngineVariant storageValue(totalJumpCount);

			auto* engineStorage = m_character->getGameInstance()->getGameEngine()->getEngineStorage();
			if (!engineStorage->getVariant("total_jump_count", &storageValue))
			{
				engineStorage->setVariant("total_jump_count", totalJumpCount);
			}
			else
			{
				engineStorage->setVariant("total_jump_count", storageValue.asUInt() + totalJumpCount);
			}

			m_gameInstance->getGameEngine()->loadScene(nextLevel);
		}
	}

	diamond_engine::EngineStatus LevelDoor::initialize(const diamond_engine::BehaviourComponentConfig* config)
	{
		auto* componentConfig = dynamic_cast<const LevelDoorConfig*>(config);
		if (!componentConfig)
		{
			return { "LevelDoor::initialize failed. Config object is of invalid type or null", true };
		}

		m_nextLevel = componentConfig->getNextLevel();

		return { };
	}

	void LevelDoor::onCollisionEnter2D(
		const glm::vec2& resolution,
		diamond_engine::GameInstance* gameInstance,
		diamond_engine::Collider2DComponent* collider2D)
	{
		if (collider2D->getType() != diamond_engine::ColliderType::CHARACTER)
		{
			return;
		}

		m_character = gameInstance->getBehaviourComponent<BoxCharacter2D>("BoxCharacter2D");
	}

	void LevelDoor::onCollisionExit2D(
		diamond_engine::GameInstance* gameInstance,
		diamond_engine::Collider2DComponent* collider2D)
	{
		if (collider2D->getType() != diamond_engine::ColliderType::CHARACTER)
		{
			return;
		}

		m_character = nullptr;
	}
}