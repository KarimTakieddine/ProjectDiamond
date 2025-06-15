#include <pugixml.hpp>

#include <glm/vec4.hpp>

#include <QHash>
#include <QSignalMapper>
#include <QUuid>

#include <parser/GameSceneConfigParser.h>

#include "LevelConfigModel.h"
#include "MaterialComponentModel.h"
#include "TransformComponentModel.h"

namespace
{
	inline glm::vec4 colorToVec4(const QColor& color)
	{
		static constexpr float kQColorToFloat = 1.0f / 0xFF;

		return
		{
			color.red	() * kQColorToFloat,
			color.green	() * kQColorToFloat,
			color.blue	() * kQColorToFloat,
			color.alpha	() * kQColorToFloat
		};
	}

	inline QColor vec4ToColor(const glm::vec4& color)
	{
		static constexpr int kFloatToQColor = 0xFF;

		return
		{
			static_cast<int>(color.r * kFloatToQColor),
			static_cast<int>(color.g * kFloatToQColor),
			static_cast<int>(color.b * kFloatToQColor),
			static_cast<int>(color.a * kFloatToQColor)
		};
	}

	using project_diamond::LevelConfigModel;
	using project_diamond::RenderComponentModel;
	using project_diamond::TransformComponentModel;
	using project_diamond::MaterialComponentModel;

	QSharedPointer<RenderComponentModel> createTransformComponentModel()
	{
		auto result = QSharedPointer<TransformComponentModel>::create();
		return result.staticCast<RenderComponentModel>();
	}

	QSharedPointer<RenderComponentModel> createMaterialComponentModel()
	{
		auto result = QSharedPointer<MaterialComponentModel>::create();
		return result.staticCast<RenderComponentModel>();
	}

	const QHash<QString, project_diamond::LevelConfigModel::RComponentCreateFunc> renderComponentCreators =
	{
		{ QStringLiteral("Transform"),	createTransformComponentModel	},
		{ QStringLiteral("Material"),	createMaterialComponentModel	}
	};
}

namespace project_diamond
{
	LevelConfigModel::LevelConfigModel(QObject* parent /* = nullptr */) :
		QObject			(parent),
		m_signalMapper	(new QSignalMapper(this)),
		m_data			(std::make_unique<diamond_engine::GameSceneConfig>()),
		m_name			(QString::fromStdString(m_data->getName())),
		m_uuid			(QUuid::createUuid().toString())
	{
		connect(m_signalMapper, &QSignalMapper::mappedInt, this, &LevelConfigModel::instanceDataChanged);
	}

	const diamond_engine::GameSceneConfig* LevelConfigModel::getData() const
	{
		return m_data.get();
	}

	bool LevelConfigModel::setPath(const QString& path)
	{
		if (path == m_path)
		{
			return false;
		}

		m_path = path;
		emit pathChanged(path);

		diamond_engine::EngineStatus status;
		auto data = diamond_engine::parseSceneFile(path.toStdString(), &status);
		emit parseStatus(QString::fromStdString(status.message));

		if (!data)
		{
			return false;
		}

		m_color = ::vec4ToColor(data->getBackgroundColor());
		m_name = QString::fromStdString(data->getName());

		const auto& instanceConfigs = data->getInstanceConfigs();

		emit loadMaximum(static_cast<int>(instanceConfigs.size()) - 1);

		for (size_t i = 0; i < instanceConfigs.size(); ++i)
		{
			const auto& instance = instanceConfigs.at(i);
			if (!instance)
			{
				// TODO: Handle error(s)
				continue;
			}

			auto instanceModel = QSharedPointer<GameInstanceModel>::create();
			instanceModel->setName(QString::fromStdString(instance->getName()));
			instanceModel->setType(instance->getType());

			insertGameInstance(m_instances.count(), instanceModel);

			for (const auto& renderComponent : instance->getRenderConfigs())
			{
				auto it = ::renderComponentCreators.constFind(QString::fromStdString(renderComponent->getName()));
				if (it == ::renderComponentCreators.constEnd())
				{
					continue;
				}

				auto componentModel = (*it)();
				componentModel->setData(renderComponent.get());
				instanceModel->insertRenderComponent(instanceModel->getRenderComponents().count(), componentModel);
			}

			// TODO: Behaviour components!

			emit loadProgress(i);
		}

		m_data = std::move(data);

		setDirty(false);

		return true;
	}

	const QString& LevelConfigModel::getPath() const
	{
		return m_path;
	}

	bool LevelConfigModel::setName(const QString& name)
	{
		if (name == m_name)
		{
			return false;
		}

		m_data->setName(name.toStdString());
		m_name = name;

		return true;
	}

	const QString& LevelConfigModel::getName() const
	{
		return m_name;
	}

	const QString& LevelConfigModel::getUuid() const
	{
		return m_uuid;
	}

	bool LevelConfigModel::setColor(const QColor& color)
	{
		if (color == m_color)
		{
			return false;
		}

		m_data->setBackgroundColor(::colorToVec4(color));
		m_color = color;

		return true;
	}

	const QColor& LevelConfigModel::getColor() const
	{
		return m_color;
	}

	bool LevelConfigModel::isDirty() const
	{
		return m_dirty;
	}

	void LevelConfigModel::setDirty(bool dirty)
	{
		m_dirty = dirty;
	}

	bool LevelConfigModel::serialize(const QString& file)
	{
		pugi::xml_document document;
		const std::string path = file.toStdString();

		pugi::xml_node rootNode = document.root().append_child("Scene");

		// TODO: More error handling

		rootNode.append_attribute("name").set_value(getName().toStdString().c_str());

		pugi::xml_node instancesNode = rootNode.append_child("Instances");
		if (!instancesNode)
		{
			return false;
		}

		for (const auto& instance : m_instances)
		{
			pugi::xml_node instanceNode = instancesNode.append_child("Instance");
			if (!instanceNode)
			{
				return false;
			}

			instance->unparse(instanceNode);
		}

		if (!document.save_file(path.c_str(), "\t", pugi::format_indent, pugi::encoding_utf8))
		{
			emit parseStatus(QStringLiteral("Failed to serialise level: ") + m_name + QStringLiteral(" to: ") + file);
			return false;
		}

		m_path = file;
		setDirty(false);
		return true;
	}

	const QVector<QSharedPointer<GameInstanceModel>>& LevelConfigModel::getInstances() const
	{
		return m_instances;
	}

	void LevelConfigModel::insertGameInstance(qsizetype index, const QSharedPointer<GameInstanceModel>& instance)
	{
		const qsizetype currentCount = m_instances.count();
		if (index < 0 || index > currentCount)
		{
			return;
		}

		m_signalMapper->setMapping(instance.get(), index);

		connect(
			instance.get(),
			&GameInstanceModel::instanceTypeChanged,
			m_signalMapper,
			qOverload<>(&QSignalMapper::map));

		connect(
			instance.get(),
			&GameInstanceModel::renderComponentDataChanged,
			m_signalMapper,
			qOverload<>(&QSignalMapper::map));

		connect(
			instance.get(),
			&GameInstanceModel::behaviourComponentDataChanged,
			m_signalMapper,
			qOverload<>(&QSignalMapper::map));

		connect(
			instance.get(),
			&GameInstanceModel::renderComponentInserted,
			this,
			&LevelConfigModel::onRenderComponentInserted);

		connect(
			instance.get(),
			&GameInstanceModel::renderComponentRemoved,
			this,
			&LevelConfigModel::onRenderComponentRemoved);

		// TODO: Connect component inserts / removes to a local slot
		// emitting a signal based on the mapping dictated by signal
		// mapper indicating for which game instance this was performed

		m_instances.insert(index, instance);

		if (index < currentCount)
		{
			for (int i = 0; i < m_instances.count(); ++i)
			{
				m_signalMapper->setMapping(m_instances.at(i).get(), i);
			}
		}

		emit gameInstanceInserted(index, instance->getName());
	}

	void LevelConfigModel::removeGameInstance(qsizetype index)
	{
		if (index < 0 || index >= m_instances.count())
		{
			return;
		}

		m_instances.removeAt(index);

		for (int i = 0; i < m_instances.size(); ++i)
		{
			m_signalMapper->setMapping(m_instances.at(i).get(), i);
		}

		emit gameInstanceRemoved(index);
	}

	int LevelConfigModel::getInstanceIndex(QObject* sender) const
	{
		for (int i = 0; i < m_instances.count(); ++i)
		{
			if (sender == m_signalMapper->mapping(i))
			{
				return i;
			}
		}

		return -1;
	}

	void LevelConfigModel::onRenderComponentInserted(qsizetype index, const QString& name)
	{
		const int componentIndex = static_cast<int>(index);
		if (componentIndex < 0)
		{
			return;
		}

		const int instanceIndex = getInstanceIndex(sender());
		if (instanceIndex == -1)
		{
			return;
		}

		emit renderComponentInserted(instanceIndex, componentIndex, name);
	}

	void LevelConfigModel::onRenderComponentRemoved(qsizetype index)
	{
		const int componentIndex = static_cast<int>(index);
		if (componentIndex < 0)
		{
			return;
		}

		const int instanceIndex = getInstanceIndex(sender());

		if (instanceIndex == -1)
		{
			return;
		}

		emit renderComponentRemoved(instanceIndex, componentIndex);
	}
}