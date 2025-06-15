#pragma once

#include <QHash>
#include <QObject>
#include <QSharedPointer>
#include <QString>

#include "LevelConfigTreeModel.h"

namespace project_diamond
{
	class LevelConfigTreeContainer : public QObject
	{
	public:
		LevelConfigTreeContainer(QObject* parent = nullptr);

		LevelConfigTreeModel* getTreeModel(const QString& key);
		LevelConfigTreeModel* insertTreeModel(const QString& key);
		void removeTreeModel(const QString& key);

	private:
		QHash<QString, QSharedPointer<LevelConfigTreeModel>> m_data;
	};
}
