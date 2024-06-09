#pragma once

#include <AudioEngine.h>
#include <ComponentFactory.h>
#include <ComponentConfigParser.h>
#include <EngineConfigParser.h>
#include <GameEngine.h>
#include <GameSceneConfigParser.h>
#include <LogManager.h>

#include "Character2D.h"
#include "Character2DConfigParser.h"
#include "DuckCharacter.h"
#include "DuckCharacterConfigParser.h"
#include "ScrollingBackground.h"
#include "ScrollingBackgroundConfigParser.h"

int main(int argc, char** argv) {
	try {
		diamond_engine::ComponentConfigParser::registerBehaviourConfig(
			"Character2D",
			&project_diamond::Character2DConfigParser::parse);

		diamond_engine::ComponentFactory::registerBehaviourComponent(
			"Character2D",
			[]() { return std::make_unique<project_diamond::Character2D>(); });

		diamond_engine::ComponentConfigParser::registerBehaviourConfig(
			"DuckCharacter",
			&project_diamond::DuckCharacterConfigParser::parse);

		diamond_engine::ComponentFactory::registerBehaviourComponent(
			"DuckCharacter",
			[]() { return std::make_unique<project_diamond::DuckCharacter>(); });

		diamond_engine::ComponentConfigParser::registerBehaviourConfig(
			"ScrollingBackground",
			&project_diamond::ScrollingBackgroundConfigParser::parse);

		diamond_engine::ComponentFactory::registerBehaviourComponent(
			"ScrollingBackground",
			[]() { return std::make_unique<project_diamond::ScrollingBackground>(); });

		diamond_engine::EngineStatus status;
		auto sceneConfig = diamond_engine::parseSceneFile("./scenes/testScene.xml", &status);

		std::unique_ptr<diamond_engine::GameEngine> gameEngine = std::make_unique<diamond_engine::GameEngine>();
		gameEngine->initialize(diamond_engine::EngineConfigParser::ParseFromFile("./config/engineConfig.xml"));
		gameEngine->addScene("testScene", "./scenes/testScene.xml");
		gameEngine->loadScene("testScene");
		gameEngine->loadScene("testScene");
		gameEngine->loadScene("testScene");
		gameEngine->loadScene("testScene");
		gameEngine->loadScene("testScene");
		gameEngine->run();
	}
	catch (const std::exception& e) {
		LOG_ERROR(e.what());

		std::this_thread::sleep_for(std::chrono::seconds(10));
	}

	return 0;
}