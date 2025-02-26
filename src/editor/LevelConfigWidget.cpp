#include "LevelConfigWidget.h"
#include "ui_LevelConfigWidget.h"

namespace project_diamond
{
	LevelConfigWidget::LevelConfigWidget(QWidget* parent)
		: QWidget(parent)
		, ui(new Ui::LevelConfigWidget())
	{
		ui->setupUi(this);
	}

	LevelConfigWidget::~LevelConfigWidget()
	{
		delete ui;
	}
}