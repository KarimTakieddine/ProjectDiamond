#pragma once

#include <QWidget>
QT_BEGIN_NAMESPACE
namespace Ui { class ComponentEditorWidget; };
QT_END_NAMESPACE

class ComponentEditorWidget : public QWidget
{
	Q_OBJECT

public:
	ComponentEditorWidget(QWidget *parent = nullptr);
	~ComponentEditorWidget();

private:
	Ui::ComponentEditorWidget *ui;
};
