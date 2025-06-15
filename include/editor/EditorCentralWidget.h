#pragma once

#include <memory>

#include <QProgressBar>
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
	class LevelConfigTreeModel;
	class TextureModel;
	class EditorCentralWidget : public QWidget
	{
		Q_OBJECT

	public:
		EditorCentralWidget(QWidget* parent = nullptr);

		void setupUi();
		void connectUi();
		void setEngineConfig(const diamond_engine::EngineConfig& config);
		void setTextureModel(TextureModel* model);

		~EditorCentralWidget();

	signals:
		void levelSelectionChanged(const LevelConfigModel* levelConfig);
		void levelDataChanged(const LevelConfigModel* levelConfig);
		void saveLevelTriggered();
		void saveLevelAsTriggered();
		void loadLevelsTriggered();

	private slots:
		void onLevelLoadMaximum(int maximum);
		void onLevelLoadProgress(int progress);

	private:
		std::unique_ptr<diamond_engine::GameEngine> m_gameEngine{ nullptr };
		Ui::EditorCentralWidget* m_ui							{ nullptr };
		QProgressBar* m_progressBar								{ nullptr };
		EditorGameWindow* m_gameWindow							{ nullptr };
		LevelConfigWidget* m_levelWidget						{ nullptr };
	};
}
