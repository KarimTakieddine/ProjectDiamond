#include <QHBoxLayout>

#include "EditorCentralWidget.h"
#include "ui_EditorCentralWidget.h"

namespace project_diamond
{
	EditorCentralWidget::EditorCentralWidget(const diamond_engine::EngineConfig& engineConfig, QWidget* parent /* = nullptr */)
		: QWidget(parent)
		, m_gameEngine(std::make_unique<diamond_engine::GameEngine>())
		, m_ui(new Ui::EditorCentralWidget())
		, m_gameWindow(new EditorGameWindow())
	{
		m_ui->setupUi(this);

		m_gameWindow->setGameEngine(std::move(m_gameEngine));
		m_gameWindow->setEngineConfig(engineConfig);
		m_gameWindow->makeCurrent();

		QHBoxLayout* levelPreviewLayout = new QHBoxLayout();
		QWidget* levelPreviewContainer	= QWidget::createWindowContainer(m_gameWindow);
		levelPreviewLayout->addWidget(levelPreviewContainer);
		levelPreviewLayout->setContentsMargins(0, 0, 0, 0);
		m_ui->levelPreviewGroupBox->setLayout(levelPreviewLayout);
	}

	EditorCentralWidget::~EditorCentralWidget()
	{
		delete m_gameWindow;
		delete m_ui;
	}
}