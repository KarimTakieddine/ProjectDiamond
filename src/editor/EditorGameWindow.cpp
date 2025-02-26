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
		surfaceFormat.setSwapInterval(0);
		surfaceFormat.setSwapBehavior(QSurfaceFormat::SwapBehavior::DoubleBuffer);
		
		setFormat(surfaceFormat);
	}

	void EditorGameWindow::setGameEngine(std::unique_ptr<diamond_engine::GameEngine> gameEngine)
	{
		m_gameEngine = std::move(gameEngine);
	}

	void EditorGameWindow::setEngineConfig(const diamond_engine::EngineConfig& config)
	{
		m_engineConfig = config;
	}

	void EditorGameWindow::initializeGL()
	{
		if (!m_gameEngine)
		{
			return;
		}

		m_gameEngine->initialize(m_engineConfig);
		m_gameEngine->loadScene(diamond_engine::LevelLoader::getInstance().getLevel("0"));

		resizeGL(width(), height());

		m_deltaTime	= 0.0f;
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
		if (m_gameEngine)
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

	void EditorGameWindow::paintOverGL()
	{
		m_deltaTimer.End();

		m_deltaTime = m_deltaTimer.GetDeltaTimeSeconds();
	}
}