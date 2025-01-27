#pragma once

#include <AudioEngine.h>
#include <ComponentFactory.h>
#include <ComponentConfigParser.h>
#include <EngineConfigParser.h>
#include <GameEngine.h>
#include <GameSceneConfigParser.h>
#include <LogManager.h>

#include "BoxCharacter2D.h"
#include "BoxCharacter2DConfigParser.h"
#include "JumpResetBox.h"
#include "JumpResetBoxConfigParser.h"
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

		diamond_engine::ComponentConfigParser::registerBehaviourConfig(
			"BoxCharacter2D",
			&project_diamond::BoxCharacter2DConfigParser::parse);

		diamond_engine::ComponentConfigParser::registerBehaviourConfig(
			"JumpResetBox",
			&project_diamond::JumpResetBoxConfigParser::parse);

		diamond_engine::EngineStatus status;
		auto sceneConfig = diamond_engine::parseSceneFile("./scenes/testScene.xml", &status);

		std::unique_ptr<diamond_engine::GameEngine> gameEngine = std::make_unique<diamond_engine::GameEngine>();
		gameEngine->initialize(diamond_engine::EngineConfigParser::ParseFromFile("./config/engineConfig.xml"));
		gameEngine->addScene("demoScene", "./scenes/characterController2DDemoScene.xml");
		gameEngine->loadScene("demoScene");
		gameEngine->run();
	}
	catch (const std::exception& e) {
		LOG_ERROR(e.what());

		std::this_thread::sleep_for(std::chrono::seconds(10));
	}

	return 0;
}