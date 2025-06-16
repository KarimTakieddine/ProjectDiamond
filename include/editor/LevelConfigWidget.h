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
	class LevelConfigTreeContainer;
	class ComponentEditorWidget;
	class TextureModel;
	class LevelConfigWidget : public QWidget
	{
		Q_OBJECT

	public:
		LevelConfigWidget(QWidget* parent = nullptr);

		void setupUi();
		void connectUi();

		void setTextureModel(TextureModel* model);

		~LevelConfigWidget();

	public slots:
		void loadLevels();
		void saveCurrentLevel();
		void saveCurrentLevelAs();

	signals:
		void levelSelectionChanged(const LevelConfigModel*);
		void levelDataChanged(const LevelConfigModel*);
		void levelLoadMaximum(int);
		void levelLoadProgress(int);

	private slots:
		void onLevelSelectionChanged(const QItemSelection& selected);
		void onLevelDataChanged(const QModelIndex& index);
		void onTableDoubleClicked(const QModelIndex& index);
		void onInsertNewBeforeClicked();
		void onInsertNewAfterClicked();
		void onRemoveCurrentClicked();
		void onClearAllClicked();
		void onRowsRemoved();
		void onRowsAboutToBeRemoved(const QModelIndex& parent, int first, int last);
		void onRowsInserted(const QModelIndex& parent, int first, int last);
		void onRenderConfigSelected(int componentIndex, int instanceIndex);
		void onBehaviourConfigSelected(int componentIndex, int instanceIndex);

	private:
		Ui::LevelConfigWidget* m_ui					{ nullptr };
		ComponentEditorWidget* m_componentEditor	{ nullptr };
		LevelConfigListModel* m_listModel			{ nullptr };
		LevelConfigTreeContainer* m_treeContainer	{ nullptr };
		TextureModel* m_textureModel				{ nullptr };
	};
}
