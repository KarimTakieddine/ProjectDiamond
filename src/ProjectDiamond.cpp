#pragma once

#include <diamond_engine/engine/GraphicsContext.h>
#include <diamond_engine/utility/LogManager.h>
#include <diamond_engine/scene/SceneLoader.h>
#include <diamond_engine/shader/SharedShaderStore.h>
#include <diamond_engine/texture/TextureLoader.h>

int main(int argc, char** argv) {
	try {
		std::unique_ptr<diamond_engine::GraphicsContext> graphicsContext = std::make_unique<diamond_engine::GraphicsContext>();
		graphicsContext->InitializeWindow({ 1280, 960 }, "Hello!");
		graphicsContext->InitializeGLEW();

		std::shared_ptr<diamond_engine::SharedShaderStore> sharedShaderStore = std::make_shared<diamond_engine::SharedShaderStore>();
		sharedShaderStore->Load("shaders");

		std::shared_ptr<diamond_engine::TextureLoader> sharedTextureLoader = std::make_shared<diamond_engine::TextureLoader>();
		//sharedTextureLoader->Load("textures");

		std::shared_ptr<diamond_engine::SceneLoader> sceneLoader = std::make_shared<diamond_engine::SceneLoader>();
		sceneLoader->SetSharedShaderStore(sharedShaderStore);
		sceneLoader->SetSharedTextureLoader(sharedTextureLoader);
		sceneLoader->LoadScenes("scenes");

		diamond_engine::Scene* sampleScene = sceneLoader->FindScene("sample_scene");
		graphicsContext->SetScene(sampleScene);

		graphicsContext->Execute();
	}
	catch (const std::exception& e) {
		LOG_ERROR(e.what());

		std::this_thread::sleep_for(std::chrono::seconds(10));
	}

	return 0;
}