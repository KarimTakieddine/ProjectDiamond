#pragma once

#include <memory>

#include <QWidget>

#include <config/EngineConfig.h>
#include <engine/GameEngine.h>

#include "EditorGameWindow.h"

QT_BEGIN_NAMESPACE
namespace Ui { class EditorCentralWidget; };
QT_END_NAMESPACE

namespace project_diamond
{
	class EditorCentralWidget : public QWidget
	{
		Q_OBJECT

	public:
		EditorCentralWidget(const diamond_engine::EngineConfig& engineConfig, QWidget* parent = nullptr);

		~EditorCentralWidget();

	private:
		std::unique_ptr<diamond_engine::GameEngine> m_gameEngine{ nullptr };
		Ui::EditorCentralWidget* m_ui{ nullptr };
		EditorGameWindow* m_gameWindow{ nullptr };
	};
}
