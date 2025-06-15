#include "LevelConfigTreeContainer.h"

namespace project_diamond
{
	LevelConfigTreeContainer::LevelConfigTreeContainer(QObject* parent /* = nullptr */) : QObject(parent) { }

	LevelConfigTreeModel* LevelConfigTreeContainer::getTreeModel(const QString& key)
	{
		auto it = m_data.constFind(key);
		if (it == m_data.constEnd())
		{
			return nullptr;
		}

		return it->get();
	}

	LevelConfigTreeModel* LevelConfigTreeContainer::insertTreeModel(const QString& key)
	{
		auto result = QSharedPointer<LevelConfigTreeModel>::create();
		m_data.insert(key, result);
		return result.get();
	}

	void LevelConfigTreeContainer::removeTreeModel(const QString& key)
	{
		auto it = m_data.constFind(key);
		if (it == m_data.constEnd())
		{
			return;
		}

		m_data.erase(it);
	}
}