#pragma once

#include <future>

#include <audio/AudioEngine.h>
#include <component/ComponentFactory.h>
#include <engine/GameEngine.h>
#include <parser/ComponentConfigParser.h>
#include <parser/EngineConfigParser.h>
#include <parser/GameSceneConfigParser.h>
#include <utility/LogManager.h>

#ifdef BUILD_EDITOR
#include <QApplication>
#include <QMainWindow>
#include "EditorCentralWidget.h"
#else
#include <engine/GLFWWindow.h>
#endif

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

		const auto engineConfig	= diamond_engine::EngineConfigParser::ParseFromFile("./config/engineConfig.xml");
		const auto& windowConfig	= engineConfig.GetWindowConfig();
		
#ifdef BUILD_EDITOR
		QApplication qApplication(argc, argv);

		project_diamond::EditorCentralWidget editorCentralWidget(engineConfig);

		int status = 0;
		auto loadStatus = levelLoadFuture.get();
		if (loadStatus)
		{
			editorCentralWidget.show();

			status = qApplication.exec();
		}
		else
		{
			status = 1;
		}

		return status;
#else
		std::unique_ptr<diamond_engine::GLFWWindow> glfwWindow = std::make_unique<diamond_engine::GLFWWindow>(windowConfig.GetSize(), windowConfig.GetTitle());
		std::unique_ptr<diamond_engine::GameEngine> gameEngine = std::make_unique<diamond_engine::GameEngine>();

		glfwWindow->setResizeHandler(std::bind(&diamond_engine::GameEngine::onWindowResize, gameEngine.get(), std::placeholders::_1));
		glfwWindow->setUpdateHandler(std::bind(&diamond_engine::GameEngine::onWindowUpdate, gameEngine.get(), std::placeholders::_1));
		gameEngine->initialize(engineConfig);
		gameEngine->onWindowResize(glfwWindow->getCurrentSize());

		auto status = levelLoadFuture.get();
		if (status)
		{
			gameEngine->loadScene(diamond_engine::LevelLoader::getInstance().getLevel("0"));
			glfwWindow->StartUpdateLoop();
			gameEngine->cleanup();
		}
		else
		{
			LOG_ERROR(status.message);
			std::this_thread::sleep_for(std::chrono::seconds(10));
			return 1;
		}
#endif
	}
	catch (const std::exception& e) {
		LOG_ERROR(e.what());
		std::this_thread::sleep_for(std::chrono::seconds(10));
		return 1;
	}

	return 0;
}