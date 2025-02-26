#pragma once

#include <memory>
#include <vector>

#include <config/GameSceneConfig.h>

#include <QAbstractItemModel>

namespace project_diamond
{
	class LevelConfigListModel : public QAbstractItemModel
	{
		Q_OBJECT

	public:
		LevelConfigListModel(QObject* parent = nullptr);

		QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const final override;
		QModelIndex parent(const QModelIndex& index) const final override;
		int rowCount(const QModelIndex& parent = QModelIndex()) const final override;
		int columnCount(const QModelIndex& parent = QModelIndex()) const final override;
		QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const final override;

		bool loadLevels(const QString& sceneDirectory);

	private:
		std::vector<std::unique_ptr<diamond_engine::GameSceneConfig>> m_levelConfigs;
	};
}
