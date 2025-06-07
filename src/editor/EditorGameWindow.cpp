#include <EngineMacros.h>
#include <debug/Debugger.h>
#include <game/LevelLoader.h>

#include "EditorGameWindow.h"

namespace project_diamond
{
	EditorGameWindow::~EditorGameWindow()
	{
		m_gameEngine->cleanup();
	}

	EditorGameWindow::EditorGameWindow(QWindow* parent /* = nullptr */) :
		QOpenGLWindow(QOpenGLWindow::UpdateBehavior::NoPartialUpdate, parent)
	{
		setMinimumSize({ 640, 360 });

		auto surfaceFormat = format();

		surfaceFormat.setMajorVersion(4);
		surfaceFormat.setMinorVersion(6);
		surfaceFormat.setProfile(QSurfaceFormat::CoreProfile);
		surfaceFormat.setSwapInterval(1);
		surfaceFormat.setSwapBehavior(QSurfaceFormat::SwapBehavior::DoubleBuffer);
		
		setFormat(surfaceFormat);

		connect(this, &EditorGameWindow::frameSwapped, this, &EditorGameWindow::onFrameSwapped);
	}

	void EditorGameWindow::setGameEngine(std::unique_ptr<diamond_engine::GameEngine> gameEngine)
	{
		m_gameEngine = std::move(gameEngine);
	}

	void EditorGameWindow::setEngineConfig(const diamond_engine::EngineConfig& config)
	{
		m_engineConfig = config;
	}

	void EditorGameWindow::loadLevel(LevelConfigModel* config)
	{
		unloadCurrentLevel();

		if (!config)
		{
			return;
		}

		auto* data = config->getData();

		if (!data)
		{
			return;
		}

		m_gameEngine->loadScene(data);
		m_deltaTime = 0.0f;
	}

	void EditorGameWindow::onFrameSwapped()
	{
		m_deltaTimer.End();

		m_deltaTime = m_deltaTimer.GetDeltaTimeSeconds();
	}

	void EditorGameWindow::initializeGL()
	{
		if (!m_gameEngine)
		{
			return;
		}

		m_gameEngine->initialize(m_engineConfig);

		resizeGL(width(), height());
	}

	void EditorGameWindow::resizeGL(int w, int h)
	{
		if (!m_gameEngine)
		{
			return;
		}

		m_gameEngine->onWindowResize({ w, h });
	}

	void EditorGameWindow::paintGL()
	{
		if (m_gameEngine && m_gameEngine->getCurrentScene() != "Unknown")
		{
			DEBUG_EXEC(diamond_engine::Debugger::getInstance()->handleAllEvents());
			m_gameEngine->onWindowUpdate(m_deltaTime);
		}

		update();
	}

	void EditorGameWindow::paintUnderGL()
	{
		m_deltaTimer.Start();
	}

	void EditorGameWindow::unloadCurrentLevel()
	{
		DEBUG_EXEC(diamond_engine::Debugger::getInstance()->purgeOutstandingEvents());
		m_gameEngine->unloadCurrentScene();
	}
}