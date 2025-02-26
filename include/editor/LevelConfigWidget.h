#pragma once

#include <QWidget>
QT_BEGIN_NAMESPACE
namespace Ui { class LevelConfigWidget; };
QT_END_NAMESPACE

namespace project_diamond
{
	class LevelConfigWidget : public QWidget
	{
		Q_OBJECT

	public:
		LevelConfigWidget(QWidget* parent = nullptr);

		~LevelConfigWidget();

	private:
		Ui::LevelConfigWidget* ui;
	};
}
