#pragma once

#include <future>

#include <AudioEngine.h>
#include <ComponentFactory.h>
#include <ComponentConfigParser.h>
#include <EngineConfigParser.h>
#include <GameEngine.h>
#include <GameSceneConfigParser.h>
#include <LogManager.h>

#include "BoxCharacter2D.h"
#include "BoxCharacter2DConfigParser.h"
#include "DeathBlock.h"
#include "DeathBlockConfigParser.h"
#include "JumpResetBox.h"
#include "JumpResetBoxConfigParser.h"
#include "LevelDoor.h"
#include "LevelDoorConfigParser.h"
#include "LevelLoader.h"
#include "ScrollingBackground.h"
#include "ScrollingBackgroundConfigParser.h"

int main(int argc, char** argv) {
	try {
		diamond_engine::ComponentConfigParser::registerBehaviourConfig(
			"ScrollingBackground",
			&project_diamond::ScrollingBackgroundConfigParser::parse);

		diamond_engine::ComponentFactory::registerBehaviourComponent(
			"ScrollingBackground",
			[]() { return std::make_unique<project_diamond::ScrollingBackground>(); });

		diamond_engine::ComponentFactory::registerBehaviourComponent(
			"BoxCharacter2D",
			[]() { return std::make_unique<project_diamond::BoxCharacter2D>(); });

		diamond_engine::ComponentFactory::registerBehaviourComponent(
			"JumpResetBox",
			[]() { return std::make_unique<project_diamond::JumpResetBox>(); });

		diamond_engine::ComponentFactory::registerBehaviourComponent(
			"LevelDoor",
			[]() { return std::make_unique<project_diamond::LevelDoor>(); });

		diamond_engine::ComponentFactory::registerBehaviourComponent(
			"DeathBlock",
			[]() { return std::make_unique<project_diamond::DeathBlock>(); });

		diamond_engine::ComponentConfigParser::registerBehaviourConfig(
			"BoxCharacter2D",
			&project_diamond::BoxCharacter2DConfigParser::parse);

		diamond_engine::ComponentConfigParser::registerBehaviourConfig(
			"JumpResetBox",
			&project_diamond::JumpResetBoxConfigParser::parse);

		diamond_engine::ComponentConfigParser::registerBehaviourConfig(
			"LevelDoor",
			&project_diamond::LevelDoorConfigParser::parse);

		diamond_engine::ComponentConfigParser::registerBehaviourConfig(
			"DeathBlock",
			&project_diamond::DeathBlockConfigParser::parse);

		auto levelLoadFuture = std::async(
			std::launch::async,
			[]() -> diamond_engine::EngineStatus { return diamond_engine::LevelLoader::getInstance().loadLevels("./scenes"); });

		std::unique_ptr<diamond_engine::GameEngine> gameEngine = std::make_unique<diamond_engine::GameEngine>();
		gameEngine->initialize(diamond_engine::EngineConfigParser::ParseFromFile("./config/engineConfig.xml"));

		auto status = levelLoadFuture.get();
		if (status)
		{
			gameEngine->loadScene(diamond_engine::LevelLoader::getInstance().getLevel("2"));
			gameEngine->run();
		}
		else
		{
			LOG_ERROR(status.message);
			std::this_thread::sleep_for(std::chrono::seconds(10));
			return 1;
		}
	}
	catch (const std::exception& e) {
		LOG_ERROR(e.what());
		std::this_thread::sleep_for(std::chrono::seconds(10));
		return 1;
	}

	return 0;
}