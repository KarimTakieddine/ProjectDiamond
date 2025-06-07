#pragma once

#include <QMainWindow>
QT_BEGIN_NAMESPACE
namespace Ui { class EditorMainWindow; };
QT_END_NAMESPACE

#include <config/EngineConfig.h>

namespace project_diamond
{
	class EditorCentralWidget;
	class EditorMainWindow : public QMainWindow
	{
		Q_OBJECT

	public:
		EditorMainWindow(QWidget* parent = nullptr);

		void setEngineConfig(const diamond_engine::EngineConfig& engineConfig);

		void setupUi();
		void connectUi();

		~EditorMainWindow();

	private:
		Ui::EditorMainWindow* m_ui{ nullptr };
		EditorCentralWidget* m_centralWidget{ nullptr };
	};
}
