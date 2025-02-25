#pragma once

#include <memory>

#include <config/EngineConfig.h>
#include <engine/GameEngine.h>
#include <utility/DeltaTimer.h>

#include <QOpenGLWindow>
#include <QWindow>

namespace project_diamond
{
	class EditorGameWindow : public QOpenGLWindow
	{
	public:
		~EditorGameWindow() final override;

		explicit EditorGameWindow(QWindow* parent = nullptr);

		void setGameEngine(std::unique_ptr<diamond_engine::GameEngine> gameEngine);
		void setEngineConfig(const diamond_engine::EngineConfig& config);

	protected:
		void initializeGL() final override;
		void resizeGL(int w, int h) final override;
		void paintGL() final override;
		void paintUnderGL() final override;
		void paintOverGL() final override;

	private:
		diamond_engine::EngineConfig m_engineConfig;
		std::unique_ptr<diamond_engine::GameEngine> m_gameEngine{ nullptr };
		diamond_engine::DeltaTimer m_deltaTimer{ };
		float m_deltaTime{ 0.0f };
	};
}
