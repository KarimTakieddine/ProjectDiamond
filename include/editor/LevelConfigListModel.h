#pragma once

#include <QAbstractItemModel>
#include <QSharedPointer>
#include <QSignalMapper>
#include <QVector>

#include "LevelConfigModel.h"

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
		QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const final override;
		bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole) final override;
		bool insertRows(int row, int count, const QModelIndex& parent = QModelIndex()) final override;
		bool removeRows(int row, int count, const QModelIndex& parent = QModelIndex()) final override;
		Qt::ItemFlags flags(const QModelIndex& index) const final override;

		bool loadLevels(const QString& sceneDirectory);

	private slots:
		void onInstanceDataChanged(int levelIndex);

	private:
		QVector<QSharedPointer<LevelConfigModel>> m_data;
		QSignalMapper* m_signalMapper{ nullptr };
	};
}
