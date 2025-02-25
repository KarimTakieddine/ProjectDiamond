#include "EditorCentralWidget.h"
#include "ui_EditorCentralWidget.h"

EditorCentralWidget::EditorCentralWidget(QWidget *parent)
	: QWidget(parent)
	, ui(new Ui::EditorCentralWidget())
{
	ui->setupUi(this);
}

EditorCentralWidget::~EditorCentralWidget()
{
	delete ui;
}
