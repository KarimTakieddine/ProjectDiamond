#pragma once

#include <QWidget>
QT_BEGIN_NAMESPACE
namespace Ui { class ComponentEditorWidget; };
QT_END_NAMESPACE

namespace project_diamond
{
	class ComponentEditorWidget : public QWidget
	{
		Q_OBJECT

	public:
		ComponentEditorWidget(QWidget* parent = nullptr);

		void setupUi();
		void connectUi();

	private:

	};
}
