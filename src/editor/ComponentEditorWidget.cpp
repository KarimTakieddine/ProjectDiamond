#include "ComponentEditorWidget.h"
#include "ui_ComponentEditorWidget.h"

ComponentEditorWidget::ComponentEditorWidget(QWidget *parent)
	: QWidget(parent)
	, ui(new Ui::ComponentEditorWidget())
{
	ui->setupUi(this);
}

ComponentEditorWidget::~ComponentEditorWidget()
{
	delete ui;
}
