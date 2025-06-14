#include <QSignalMapper>

#include <config/RenderComponentConfig.h>

#include "GameInstanceModel.h"
#include "MaterialComponentModel.h"
#include "TransformComponentModel.h"

namespace
{
	using project_diamond::RenderComponentModel;
	using project_diamond::TransformComponentModel;
	using project_diamond::MaterialComponentModel;
	using project_diamond::GameInstanceModel;

	void connectTransformComponent(GameInstanceModel* instance, const QSharedPointer<RenderComponentModel>& model, qsizetype index)
	{
		auto* componentModel = dynamic_cast<TransformComponentModel*>(model.get());
		if (!componentModel)
		{
			return;
		}

		auto* signalMapper = instance->getRenderSignalMapper();

		QObject::connect(
			componentModel,
			&TransformComponentModel::positionChanged,
			signalMapper,
			qOverload<>(&QSignalMapper::map));

		QObject::connect(
			componentModel,
			&TransformComponentModel::localScaleChanged,
			signalMapper,
			qOverload<>(&QSignalMapper::map));

		signalMapper->setMapping(componentModel, static_cast<int>(index));
	}

	void connectMaterialomponent(GameInstanceModel* instance, const QSharedPointer<RenderComponentModel>& model, qsizetype index)
	{
		auto* componentModel = dynamic_cast<MaterialComponentModel*>(model.get());
		if (!componentModel)
		{
			return;
		}

		auto* signalMapper = instance->getRenderSignalMapper();

		QObject::connect(
			componentModel,
			&MaterialComponentModel::colorChanged,
			signalMapper,
			qOverload<>(&QSignalMapper::map));

		QObject::connect(
			componentModel,
			&MaterialComponentModel::textureOffsetChanged,
			signalMapper,
			qOverload<>(&QSignalMapper::map));

		QObject::connect(
			componentModel,
			&MaterialComponentModel::textureNameChanged,
			signalMapper,
			qOverload<>(&QSignalMapper::map));

		signalMapper->setMapping(componentModel, static_cast<int>(index));
	}

	const QHash<QString, GameInstanceModel::RComponentConnectFunc> componentConnectFunctions =
	{
		{ QStringLiteral("Transform"), connectTransformComponent },
		{ QStringLiteral("Material"), connectMaterialomponent }
	};

	using diamond_engine::GameInstanceType;

	QString getTypeString(diamond_engine::GameInstanceType type)
	{
		switch (type)
		{
		case diamond_engine::GameInstanceType::SPRITE:
			return QStringLiteral("sprite");
		case diamond_engine::GameInstanceType::COLLIDER_2D:
			return QStringLiteral("collider2D");
		default:
			break;
		}

		return QString();
	}
}

namespace project_diamond
{
	GameInstanceModel::GameInstanceModel(QObject* parent /* = nullptr */) :
		QObject(parent),
		m_renderSignalMapper(new QSignalMapper(this)),
		m_behaviourSignalMapper(new QSignalMapper(this))
	{
		connect(m_renderSignalMapper, &QSignalMapper::mappedInt, this, &GameInstanceModel::renderComponentDataChanged);
		connect(m_behaviourSignalMapper, &QSignalMapper::mappedInt, this, &GameInstanceModel::behaviourComponentDataChanged);
	}

	QSignalMapper* GameInstanceModel::getRenderSignalMapper() const
	{
		return m_renderSignalMapper;
	}

	diamond_engine::GameInstanceType GameInstanceModel::getType() const
	{
		return m_type;
	}

	const QString& GameInstanceModel::getName() const
	{
		return m_name;
	}

	void GameInstanceModel::insertRenderComponent(qsizetype index, const QSharedPointer<RenderComponentModel>& component)
	{
		const qsizetype currentCount = m_renderComponents.count();
		if (index < 0 || index > currentCount)
		{
			return;
		}

		const auto* data = component->getData();
		const QString name = data ? QString::fromStdString(data->getName()) : QStringLiteral("null");

		auto it = ::componentConnectFunctions.constFind(name);
		if (it != ::componentConnectFunctions.constEnd())
		{
			(*it)(this, component, index);
		}

		m_renderComponents.insert(index, component);

		if (index < currentCount)
		{
			for (int i = 0; i < m_renderComponents.count(); ++i)
			{
				m_renderSignalMapper->setMapping(m_renderComponents.at(i).get(), i);
			}
		}

		emit renderComponentInserted(index, name);
	}

	void GameInstanceModel::removeRenderComponent(qsizetype index)
	{
		if (index < 0 || index >= m_renderComponents.size())
		{
			return;
		}

		const auto& component = m_renderComponents.at(index);
		const auto* data = component->getData();
		const QString name = data ? QString::fromStdString(data->getName()) : QStringLiteral("null");

		m_renderComponents.removeAt(index);

		for (int i = 0; i < m_renderComponents.size(); ++i)
		{
			m_renderSignalMapper->setMapping(m_renderComponents.at(i).get(), i);
		}

		emit renderComponentRemoved(index, name);
	}

	const QVector<QSharedPointer<RenderComponentModel>>& GameInstanceModel::getRenderComponents() const
	{
		return m_renderComponents;
	}

	bool GameInstanceModel::unparse(pugi::xml_node& node) const
	{
		// TODO name and type

		node.append_attribute("name").set_value(m_name.toStdString().c_str());
		node.append_attribute("type").set_value(::getTypeString(m_type).toStdString().c_str());

		pugi::xml_node renderComponentsNode = node.append_child("RenderComponents");
		for (const auto& renderComponent : m_renderComponents)
		{
			pugi::xml_node componentNode = node.append_child();
			if (!componentNode)
			{
				return false;
			}

			renderComponent->unparse(componentNode);
		}

		return true;
	}

	void GameInstanceModel::setType(diamond_engine::GameInstanceType type)
	{
		if (type == m_type)
		{
			return;
		}

		m_type = type;

		emit instanceTypeChanged(type);
	}

	void GameInstanceModel::setName(const QString& name)
	{
		if (name == m_name)
		{
			return;
		}

		m_name = name;

		emit nameChanged(name);
	}
}