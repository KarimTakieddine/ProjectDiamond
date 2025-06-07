#include "LevelConfigModel.h"
#include "LevelConfigTreeModel.h"

namespace project_diamond
{
	/* static */ const QHash<QString, LevelConfigTreeModel::EmitSignalFunc> LevelConfigTreeModel::signalsToEmit =
	{
		{ QStringLiteral("Render"),		&LevelConfigTreeModel::renderConfigSelected },
		{ QStringLiteral("Behaviour"),	&LevelConfigTreeModel::behaviourConfigSelected }
	};

	LevelConfigTreeModel::LevelConfigTreeModel(QObject* parent /* = nullptr */) : QStandardItemModel(parent) { }

	void LevelConfigTreeModel::loadLevelConfig(const LevelConfigModel* levelConfig)
	{
		clear();

		if (!levelConfig)
		{
			// TODO
			return;
		}

		const auto* data = levelConfig->getData();
		if (!data)
		{
			// TODO
			return;
		}

		const auto& gameInstances = data->getInstanceConfigs();
		for (size_t i = 0; i < gameInstances.size(); ++i)
		{
			const auto& gameInstance = gameInstances.at(i);

			QStandardItem* instanceItem = new QStandardItem(QString::fromStdString(gameInstance->getName()));

			QStandardItem* renderComponentsParent = new QStandardItem(QStringLiteral("Render"));
			const auto& renderComponents = gameInstance->getRenderConfigs();
			for (size_t j = 0; j < renderComponents.size(); ++j)
			{
				renderComponentsParent->appendRow(new QStandardItem(renderComponents[j]->getName()));
			}
			
			instanceItem->appendRow(renderComponentsParent);

			QStandardItem* behaviourComponentsParent = new QStandardItem(QStringLiteral("Behaviour"));
			const auto& behaviourComponents = gameInstance->getBehaviourConfigs();
			for (size_t j = 0; j < behaviourComponents.size(); ++j)
			{
				behaviourComponentsParent->appendRow(new QStandardItem(behaviourComponents[j]->getName()));
			}

			instanceItem->appendRow(behaviourComponentsParent);
			
			setItem(static_cast<int>(i), instanceItem);
		}
	}

	void LevelConfigTreeModel::onIndexSelected(const QModelIndex& selectedIndex)
	{
		if (!selectedIndex.isValid())
		{
			// TODO
			return;
		}

		const QModelIndex parentIndex = selectedIndex.parent();
		if (!parentIndex.isValid())
		{
			return;
		}

		auto* item = itemFromIndex(parentIndex);
		if (!item)
		{
			return;
		}

		auto it = signalsToEmit.constFind(item->text());
		if (it == signalsToEmit.constEnd())
		{
			return;
		}

		const QModelIndex grandParentIndex = parentIndex.parent();
		if (!grandParentIndex.isValid())
		{
			return;
		}

		emit (*it)(this, selectedIndex.row(), grandParentIndex.row());
	}
}