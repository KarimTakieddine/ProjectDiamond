#include <filesystem>

#include <parser/GameSceneConfigParser.h>

#include "LevelConfigListModel.h"

namespace project_diamond
{
	LevelConfigListModel::LevelConfigListModel(QObject* parent) : QAbstractItemModel(parent) { }

	QModelIndex LevelConfigListModel::index(int row, int column, const QModelIndex& parent) const
	{
		return QModelIndex();
	}

	QModelIndex LevelConfigListModel::parent(const QModelIndex& index) const
	{
		return QModelIndex();
	}

	int LevelConfigListModel::rowCount(const QModelIndex& parent) const
	{
		return 0;
	}

	int LevelConfigListModel::columnCount(const QModelIndex& parent) const
	{
		return 0;
	}

	QVariant LevelConfigListModel::data(const QModelIndex& index, int role) const
	{
		return QVariant();
	}

	bool LevelConfigListModel::loadLevels(const QString& sceneDirectory)
	{
		std::filesystem::path sceneDirectoryPath(sceneDirectory.toStdString());
		if (!std::filesystem::is_directory(sceneDirectoryPath))
		{
			return false;
		}

		sceneDirectoryPath.make_preferred();
		
		for (const auto& child : std::filesystem::directory_iterator(sceneDirectoryPath))
		{
			if (!child.is_regular_file())
			{
				continue;
			}


		}

		return true;
	}
}