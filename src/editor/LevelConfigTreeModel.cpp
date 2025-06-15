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

	void LevelConfigTreeModel::onGameInstanceInserted(int index, const QString& name)
	{
		QStandardItem* instanceItem = new QStandardItem(name);

		instanceItem->appendRow(new QStandardItem(QStringLiteral("Render")));
		instanceItem->appendRow(new QStandardItem(QStringLiteral("Behaviour")));

		appendRow(instanceItem);
	}

	void LevelConfigTreeModel::onGameInstanceRemoved(int index)
	{
		removeRow(index);
	}

	void LevelConfigTreeModel::onRenderComponentInserted(int instanceIndex, int componentIndex, const QString& name)
	{
		QStandardItem* instanceItem = itemFromIndex(index(instanceIndex, 0));
		if (!instanceItem)
		{
			// TODO
			return;
		}

		QStandardItem* renderItem = instanceItem->child(0);
		if (!renderItem)
		{
			// TODO
			return;
		}

		renderItem->insertRow(componentIndex, new QStandardItem(name));
	}

	void LevelConfigTreeModel::onRenderComponentRemoved(int instanceIndex, int componentIndex)
	{
		QStandardItem* instanceItem = itemFromIndex(index(instanceIndex, 0));
		if (!instanceItem)
		{
			// TODO
			return;
		}

		QStandardItem* renderItem = instanceItem->child(0);
		if (!renderItem)
		{
			// TODO
			return;
		}

		renderItem->removeRow(componentIndex);
	}

	void LevelConfigTreeModel::onIndexSelected(const QModelIndex& selectedIndex)
	{
		if (!selectedIndex.isValid())
		{
			emit renderConfigSelected(-1, -1);
			emit behaviourConfigSelected(-1, -1);
			return;
		}

		const int row = selectedIndex.row();

		const QModelIndex parentIndex = selectedIndex.parent();
		if (!parentIndex.isValid())
		{
			emit renderConfigSelected(row, -1);
			emit behaviourConfigSelected(row, -1);
			return;
		}

		auto* item = itemFromIndex(parentIndex);
		if (!item)
		{
			emit renderConfigSelected(row, -1);
			emit behaviourConfigSelected(row, -1);
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
			emit renderConfigSelected(row, -1);
			emit behaviourConfigSelected(row, -1);
			return;
		}

		emit (*it)(this, selectedIndex.row(), grandParentIndex.row());
	}
}