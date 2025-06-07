#include <algorithm>
#include <filesystem>

#include <QColor>

#include <glm/vec3.hpp>

#include <parser/GameSceneConfigParser.h>

#include "LevelConfigListModel.h"

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
		return static_cast<int>(m_data.count());
	}

	int LevelConfigListModel::columnCount(const QModelIndex& parent) const
	{
		return 3;
	}

	QVariant LevelConfigListModel::data(const QModelIndex& index, int role /* = Qt::DisplayRole */) const
	{
		if (!index.isValid())
		{
			return { };
		}

		const size_t row = static_cast<size_t>(index.row());

		if (row >= m_data.count())
		{
			// TODO: Emit with error message?

			return { };
		}

		auto& config = m_data.at(row);
		if (!config)
		{
			// TODO: Emit with error message?

			return { };
		}

		if (role == Qt::UserRole)
		{
			return QVariant::fromValue(config.get());
		}

		switch (index.column())
		{
			case 0:
			{
				switch (role)
				{
				case Qt::DisplayRole:
					return config->getName();
				default:
					break;
				}

				break;
			}
			case 1:
			{
				switch (role)
				{
				case Qt::DisplayRole:
					return config->getPath();
				default:
					break;
				}

				break;
			}
			case 2:
			{
				switch (role)
				{
				case Qt::BackgroundRole:
				case Qt::DisplayRole:
					return config->getColor();
				case Qt::ForegroundRole:
					return QColor{ 255, 255, 255 };
				default:
					break;
				}

				break;
			}
			default:
			{
				break;
			}
		}

		return { };
	}

	QVariant LevelConfigListModel::headerData(int section, Qt::Orientation orientation, int role /* = Qt::DisplayRole*/) const
	{
		if (role != Qt::DisplayRole)
		{
			return { };
		}

		switch (orientation)
		{
		case Qt::Horizontal:
		{
			switch (section)
			{
			case 0:
				return QStringLiteral("Name");
			case 1:
				return QStringLiteral("Path");
			case 2:
				return QStringLiteral("BG Color");
			default:
				break;
			}

			break;
		}
		case Qt::Vertical:
			return section;
		default:
			break;
		}

		return { };
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

		if (row >= m_data.count())
		{
			// TODO: Emit with error message?

			return false;
		}

		auto& config = m_data.at(row);
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
				const QString name = value.toString();

				if (name.isEmpty())
				{
					changed = false;
					break;
				}

				changed = config->setName(name);
				break;
			}
			case 1:
			{
				const QString path = value.toString();

				if (path.isEmpty())
				{
					changed = false;
					break;
				}

				changed = config->setPath(path);
				break;
			}
			case 2:
			{
				changed = config->setColor(value.value<QColor>());
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
			config->setDirty(true);
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

		if (row < 0 || row > rowCount())
		{
			return false;
		}

		const int lastIndex = row - 1 + count;
		beginInsertRows(parent, row, lastIndex);

		for (size_t i = row; i <= static_cast<size_t>(lastIndex); ++i)
		{
			auto config = QSharedPointer<LevelConfigModel>::create();
			m_data.insert(std::next(m_data.begin(), i), config);
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
		auto firstIt = std::next(m_data.begin(), row);
		m_data.erase(firstIt, firstIt + count);
		endRemoveRows();

		return true;
	}

	Qt::ItemFlags LevelConfigListModel::flags(const QModelIndex& index) const
	{
		if (!index.isValid())
		{
			return Qt::NoItemFlags;
		}
		
		const int row = index.row();
		if (row < 0 || row >= rowCount())
		{
			return Qt::NoItemFlags;
		}

		switch (index.column())
		{
		case 0:
			return Qt::ItemIsEnabled | Qt::ItemIsEditable | Qt::ItemIsSelectable;
		case 1:
			return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
		default:
			return Qt::NoItemFlags;
		}
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
				return false;
			}

			const QModelIndex lastIndex = index(rowCount() - 1, 0);

			auto* levelConfigModel = qvariant_cast<LevelConfigModel*>(data(lastIndex, Qt::UserRole));
			// TODO: Connect parse status
			if (levelConfigModel->setPath(QString::fromStdString(child.path().string())))
			{
				emit dataChanged(lastIndex, lastIndex);
			}
		}

		return true;
	}
}