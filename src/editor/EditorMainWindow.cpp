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
	}

	void EditorMainWindow::connectUi()
	{
		m_centralWidget->connectUi();

		connect(m_ui->actionLoadLevels, &QAction::triggered, m_centralWidget, &EditorCentralWidget::onLoadLevelsTriggered);
	}

	EditorMainWindow::~EditorMainWindow()
	{
		delete m_ui;
	}
}