#pragma once

#include <QWidget>
QT_BEGIN_NAMESPACE
namespace Ui { class EditorCentralWidget; };
QT_END_NAMESPACE

class EditorCentralWidget : public QWidget
{
	Q_OBJECT

public:
	EditorCentralWidget(QWidget *parent = nullptr);
	~EditorCentralWidget();

private:
	Ui::EditorCentralWidget *ui;
};
