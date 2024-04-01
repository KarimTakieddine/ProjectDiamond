#pragma once

/*
#include <diamond_engine/animation/SpriteSheetLoader.h>
#include <diamond_engine/audio/AudioEngine.h>
#include <diamond_engine/behaviour/BehaviourRegistry.h>
#include <diamond_engine/engine/GraphicsContext.h>
#include <diamond_engine/parser/EngineConfigParser.h>
#include <diamond_engine/rendering/GameInstanceManager.h>
#include <diamond_engine/scene/SceneLoader.h>
#include <diamond_engine/shader/SharedShaderStore.h>
#include <diamond_engine/texture/TextureLoader.h>
#include <diamond_engine/utility/LogManager.h>

#include "behaviour/CharacterController2DBehaviour.h"
#include "builder/CharacterController2DBehaviourBuilder.h"
#include "parser/CharacterController2DBehaviourConfigParser.h"
*/

#include <diamond_engine/engine/GraphicsContext.h>
#include <diamond_engine/parser/EngineConfigParser.h>
#include <diamond_engine/rendering/GameEngine.h>
#include <diamond_engine/utility/LogManager.h>

int main(int argc, char** argv) {
	try {
		//diamond_engine::RegisterBehaviour("CharacterController2DBehaviour", &CharacterController2DBehaviourConfigParser::Parse, &CharacterController2DBehaviourBuilder::Build);

		//diamond_engine::AudioEngine::instance()->initialize("audio");

		std::unique_ptr<diamond_engine::GameEngine> gameEngine = std::make_unique<diamond_engine::GameEngine>();
		gameEngine->initialize(diamond_engine::EngineConfigParser::ParseFromFile("./config/engineConfig.xml"));
		gameEngine->addScene("testScene", "./scenes/testScene.xml");
		gameEngine->loadScene("testScene");
		gameEngine->loadScene("testScene");
		gameEngine->loadScene("testScene");
		gameEngine->loadScene("testScene");
		gameEngine->loadScene("testScene");
		gameEngine->run();
		gameEngine->unloadCurrentScene();

		/*
		diamond_engine::SceneConfig sceneConfig;
		sceneConfig.SetMaxObjects(1);
		gameInstanceManager->loadScene(sceneConfig);
		gameInstanceManager->loadScene(sceneConfig);
		*/

		/*
		std::shared_ptr<diamond_engine::SpriteSheetLoader> sharedSpriteSheetLoader = std::make_shared<diamond_engine::SpriteSheetLoader>();
		sharedSpriteSheetLoader->setSharedTextureLoader(sharedTextureLoader);
		sharedSpriteSheetLoader->load("spriteSheets");

		std::shared_ptr<diamond_engine::SceneLoader> sceneLoader = std::make_shared<diamond_engine::SceneLoader>();
		sceneLoader->SetSharedShaderStore(sharedShaderStore);
		sceneLoader->SetSharedTextureLoader(sharedTextureLoader);
		sceneLoader->setSharedSpriteSheetLoader(sharedSpriteSheetLoader);
		sceneLoader->LoadScenes("scenes");

		diamond_engine::Scene* sampleScene = sceneLoader->FindScene("characterController2DDemoScene");
		graphicsContext->SetScene(sampleScene);

		graphicsContext->Execute();
		*/
	}
	catch (const std::exception& e) {
		LOG_ERROR(e.what());

		std::this_thread::sleep_for(std::chrono::seconds(10));
	}

	return 0;
}