#include "EditorCentralWidget.h"
#include "EditorMainWindow.h"
#include "ui_EditorMainWindow.h"

namespace project_diamond
{
	EditorMainWindow::EditorMainWindow(QWidget* parent)
		: QMainWindow(parent)
		, m_ui(new Ui::EditorMainWindow())
		, m_centralWidget(new EditorCentralWidget())
	{

	}

	void EditorMainWindow::setEngineConfig(const diamond_engine::EngineConfig& engineConfig)
	{
		m_centralWidget->setEngineConfig(engineConfig);
	}

	void EditorMainWindow::setupUi()
	{
		m_ui->setupUi(this);

		m_centralWidget->setupUi();
		setCentralWidget(m_centralWidget);

		m_ui->actionSaveLevel->setEnabled(false);
		m_ui->actionSaveLevelAs->setEnabled(false);
	}

	void EditorMainWindow::connectUi()
	{
		m_centralWidget->connectUi();

		connect(m_ui->actionLoadLevels, &QAction::triggered, m_centralWidget, &EditorCentralWidget::onLoadLevelsTriggered);
		connect(m_ui->actionSaveLevel, &QAction::triggered, m_centralWidget, &EditorCentralWidget::saveLevelTriggered);
		connect(m_ui->actionSaveLevelAs, &QAction::triggered, m_centralWidget, &EditorCentralWidget::saveLevelAsTriggered);
		connect(m_centralWidget, &EditorCentralWidget::levelSelectionChanged, this, &EditorMainWindow::onLevelChanged);
		connect(m_centralWidget, &EditorCentralWidget::levelDataChanged, this, &EditorMainWindow::onLevelChanged);
	}

	void EditorMainWindow::setTextureModel(TextureModel* textureModel)
	{
		m_centralWidget->setTextureModel(textureModel);
	}

	void EditorMainWindow::onLevelChanged(const LevelConfigModel* levelConfig)
	{
		if (!levelConfig)
		{
			m_ui->actionSaveLevel->setEnabled(false);
			m_ui->actionSaveLevelAs->setEnabled(false);
			return;
		}

		m_ui->actionSaveLevel->setEnabled(levelConfig->isDirty() && !levelConfig->getPath().isEmpty());
		m_ui->actionSaveLevelAs->setEnabled(true);
	}

	EditorMainWindow::~EditorMainWindow()
	{
		delete m_ui;
	}
}