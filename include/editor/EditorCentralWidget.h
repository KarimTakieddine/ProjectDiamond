#pragma once

#include <memory>

#include <QWidget>

#include <config/EngineConfig.h>
#include <engine/GameEngine.h>

#include "EditorGameWindow.h"
#include "LevelConfigWidget.h"

QT_BEGIN_NAMESPACE
namespace Ui { class EditorCentralWidget; };
QT_END_NAMESPACE

namespace project_diamond
{
	class LevelConfigListModel;
	class EditorCentralWidget : public QWidget
	{
		Q_OBJECT

	public:
		EditorCentralWidget(QWidget* parent = nullptr);

		void setupUi();
		void connectUi();
		void setEngineConfig(const diamond_engine::EngineConfig& config);

		~EditorCentralWidget();

	public slots:
		void onLoadLevelsTriggered();

	private:
		std::unique_ptr<diamond_engine::GameEngine> m_gameEngine{ nullptr };
		Ui::EditorCentralWidget* m_ui							{ nullptr };
		LevelConfigListModel* m_levelListModel					{ nullptr };
		EditorGameWindow* m_gameWindow							{ nullptr };
		LevelConfigWidget* m_levelWidget						{ nullptr };
	};
}
