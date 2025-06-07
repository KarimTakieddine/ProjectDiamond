#include <QFileDialog>
#include <QHBoxLayout>
#include <QSplitter>

#include "EditorCentralWidget.h"
#include "ui_EditorCentralWidget.h"
#include "LevelConfigListModel.h"

namespace project_diamond
{
	EditorCentralWidget::EditorCentralWidget(QWidget* parent /* = nullptr */)
		: QWidget(parent)
		, m_gameEngine(std::make_unique<diamond_engine::GameEngine>())
		, m_ui(new Ui::EditorCentralWidget())
		, m_levelListModel(new LevelConfigListModel(this))
		, m_gameWindow(new EditorGameWindow())
		, m_levelWidget(new LevelConfigWidget())
	{
	}

	void EditorCentralWidget::setupUi()
	{
		m_ui->setupUi(this);
		m_levelWidget->setupUi();
		m_levelWidget->setModel(m_levelListModel);

		QSplitter* splitter = new QSplitter();
		splitter->addWidget(m_levelWidget);

		m_gameWindow->setGameEngine(std::move(m_gameEngine));

		QHBoxLayout* levelPreviewLayout = new QHBoxLayout();
		QWidget* levelPreviewContainer = QWidget::createWindowContainer(m_gameWindow);
		levelPreviewLayout->addWidget(levelPreviewContainer);
		levelPreviewLayout->setContentsMargins(0, 0, 0, 0);
		m_ui->levelPreviewGroupBox->setLayout(levelPreviewLayout);

		splitter->addWidget(m_ui->levelPreviewGroupBox);
		splitter->setStretchFactor(1, 1);
		splitter->setStyleSheet(QStringLiteral("QSplitter::handle { background-color: #333; }"));

		m_ui->layout->addWidget(splitter);
	}

	void EditorCentralWidget::connectUi()
	{
		m_levelWidget->connectUi();

		connect(m_levelWidget, &LevelConfigWidget::levelSelectionChanged, m_gameWindow, &EditorGameWindow::loadLevel);
		connect(m_levelWidget, &LevelConfigWidget::levelSelectionChanged, this, &EditorCentralWidget::levelSelectionChanged);
		connect(m_levelWidget, &LevelConfigWidget::levelDataChanged, m_gameWindow, &EditorGameWindow::loadLevel);
		connect(m_levelWidget, &LevelConfigWidget::levelDataChanged, this, &EditorCentralWidget::levelDataChanged);
		connect(this, &EditorCentralWidget::saveLevelTriggered, m_levelWidget, &LevelConfigWidget::saveCurrentLevel);
		connect(this, &EditorCentralWidget::saveLevelAsTriggered, m_levelWidget, &LevelConfigWidget::saveCurrentLevelAs);
	}

	void EditorCentralWidget::setEngineConfig(const diamond_engine::EngineConfig& config)
	{
		m_gameWindow->setEngineConfig(config);
		m_gameWindow->makeCurrent();
	}

	void EditorCentralWidget::onLoadLevelsTriggered()
	{
		const QString directory = QFileDialog::getExistingDirectory(
			nullptr,
			QStringLiteral("Load Levels from Folder"));

		if (directory.isNull())
		{
			return;
		}

		m_levelListModel->loadLevels(directory);
	}

	EditorCentralWidget::~EditorCentralWidget()
	{
		delete m_gameWindow;
		delete m_ui;
	}
}