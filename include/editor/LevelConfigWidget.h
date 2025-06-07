#pragma once

#include <QWidget>
#include <QItemSelection>

#include "LevelConfigModel.h"

QT_BEGIN_NAMESPACE
namespace Ui { class LevelConfigWidget; };
QT_END_NAMESPACE

namespace project_diamond
{
	class LevelConfigListModel;
	class LevelConfigWidget : public QWidget
	{
		Q_OBJECT

	public:
		LevelConfigWidget(QWidget* parent = nullptr);

		void setupUi();
		void connectUi();

		void setModel(LevelConfigListModel* model);

		~LevelConfigWidget();

	public slots:
		void saveCurrentLevel();
		void saveCurrentLevelAs();

	signals:
		void levelSelectionChanged(LevelConfigModel*);
		void levelDataChanged(LevelConfigModel*);

	private slots:
		void onLevelSelectionChanged(const QItemSelection& selected);
		void onLevelDataChanged(const QModelIndex& index);
		void onTableDoubleClicked(const QModelIndex& index);
		void onInsertNewBeforeClicked();
		void onInsertNewAfterClicked();
		void onRemoveCurrentClicked();
		void onClearAllClicked();
		void onRowsRemoved();
		void onRowsInserted(const QModelIndex& parent, int first, int last);

	private:
		Ui::LevelConfigWidget* m_ui		{ nullptr };
		LevelConfigListModel* m_model	{ nullptr };
	};
}
