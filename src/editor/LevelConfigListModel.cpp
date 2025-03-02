#include <algorithm>
#include <filesystem>

#include <glm/vec3.hpp>

#include <parser/GameSceneConfigParser.h>

#include "LevelConfigListModel.h"

namespace
{
	QString getColorString(const glm::vec3& color)
	{
		return QStringLiteral("r: %1 - g: %2 - b: %3").arg(color.r).arg(color.g).arg(color.b);
	}
}

namespace project_diamond
{
	LevelConfigListModel::LevelConfigListModel(QObject* parent) : QAbstractItemModel(parent) { }

	QModelIndex LevelConfigListModel::index(int row, int column, const QModelIndex& parent) const
	{
		Q_UNUSED(parent)

		return createIndex(row, column);
	}

	QModelIndex LevelConfigListModel::parent(const QModelIndex& index) const
	{
		return QModelIndex();
	}

	int LevelConfigListModel::rowCount(const QModelIndex& parent) const
	{
		return static_cast<int>(m_levelConfigs.size());
	}

	int LevelConfigListModel::columnCount(const QModelIndex& parent) const
	{
		return 4;
	}

	QVariant LevelConfigListModel::data(const QModelIndex& index, int role) const
	{
		return QVariant();
	}

	bool LevelConfigListModel::setData(const QModelIndex& index, const QVariant& value, int role /* = Qt::EditRole */)
	{
		if (!index.isValid())
		{
			return false;
		}

		if (role != Qt::EditRole)
		{
			return false;
		}

		const size_t row = static_cast<size_t>(index.row());

		if (row >= m_levelConfigs.size())
		{
			// TODO: Emit with error message?

			return false;
		}

		auto& config = m_levelConfigs.at(row);
		if (!config)
		{
			// TODO: Emit with error message?

			return false;
		}

		bool changed = true;

		switch (index.column())
		{
			case 0:
			{
				config->setName(value.toString().toStdString());
				break;
			}
			case 1:
			{
				config->setPath(value.toString().toStdString());
				break;
			}
			default:
			{
				// TODO: Emit with error message?

				changed = false;
				break;
			}
		}

		if (changed)
		{
			emit dataChanged(index, index);
		}

		return changed;
	}

	bool LevelConfigListModel::insertRows(int row, int count, const QModelIndex& parent /* = QModelIndex() */)
	{
		if (count <= 0)
		{
			return false;
		}

		if (row < 0 || row >= rowCount())
		{
			return false;
		}

		const int lastIndex = row - 1 + count;
		beginInsertRows(parent, row, lastIndex);

		for (size_t i = row; row < static_cast<size_t>(lastIndex); ++i)
		{
			auto config = std::make_unique<diamond_engine::GameSceneConfig>();
			m_levelConfigs.insert(std::next(m_levelConfigs.begin(), i), std::move(config));
		}

		endInsertRows();

		return true;
	}

	bool LevelConfigListModel::removeRows(int row, int count, const QModelIndex& parent /* = QModelIndex() */)
	{
		if (count <= 0)
		{
			return false;
		}

		if (row < 0 || row >= rowCount())
		{
			return false;
		}

		const int lastIndex = row - 1 + count;
		beginRemoveRows(parent, row, lastIndex);
		auto firstIt = std::next(m_levelConfigs.begin(), row);
		m_levelConfigs.erase(firstIt, firstIt + count);
		endRemoveRows();

		return true;
	}

	bool LevelConfigListModel::loadLevels(const QString& sceneDirectory)
	{
		std::filesystem::path sceneDirectoryPath(sceneDirectory.toStdString());
		if (!std::filesystem::is_directory(sceneDirectoryPath))
		{
			return false;
		}

		sceneDirectoryPath.make_preferred();

		const int configCount = rowCount();
		if ( configCount > 0 )
		{
			removeRows(0, configCount);
		}
		
		for (const auto& child : std::filesystem::directory_iterator(sceneDirectoryPath))
		{
			if ( !child.is_regular_file() )
			{
				continue;
			}

			if ( !insertRow(rowCount()) )
			{
				emit loadError(QStringLiteral("LevelConfigListModel::loadLevels - Failed to add row to model"));
				return false;
			}

			diamond_engine::EngineStatus parseStatus;
			auto sceneConfig = diamond_engine::parseSceneFile(child.path().string(), &parseStatus);

			if (!sceneConfig)
			{
				emit loadError(
					QStringLiteral("LevelConfigListModel::loadLevels - Config parse failure at: %1 - Error was: %2")
					.arg(QString::fromStdString(child.path().string())).arg(QString::fromStdString(parseStatus.message)));

				continue;
			}

			const int lastRow = rowCount() - 1;
			setData(index(lastRow, 0), QString::fromStdString(sceneConfig->getName()));
			setData(index(lastRow, 1), QString::fromStdString(sceneConfig->getPath()));
			setData(index(lastRow, 2), sceneConfig->getInstanceConfigs().size());
			setData(index(lastRow, 3), getColorString(sceneConfig->getBackgroundColor()));
		}

		return true;
	}
}