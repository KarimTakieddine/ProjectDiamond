#include <QSignalMapper>

#include <config/RenderComponentConfig.h>

#include "GameInstanceModel.h"
#include "TransformComponentModel.h"

namespace
{
	using project_diamond::RenderComponentModel;
	using project_diamond::TransformComponentModel;
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

	const QHash<QString, GameInstanceModel::RComponentConnectFunc> componentConnectFunctions =
	{
		{ QStringLiteral("Transform"), connectTransformComponent }
	};
}

namespace project_diamond
{
	GameInstanceModel::GameInstanceModel(QObject* parent /* = nullptr */) :
		QObject					(parent),
		m_renderSignalMapper	(new QSignalMapper(this)),
		m_behaviourSignalMapper	(new QSignalMapper(this))
	{
		connect(m_renderSignalMapper, &QSignalMapper::mappedInt, this, &GameInstanceModel::renderComponentDataChanged);
		connect(m_behaviourSignalMapper, &QSignalMapper::mappedInt, this, &GameInstanceModel::behaviourComponentDataChanged);
	}

	QSignalMapper* GameInstanceModel::getRenderSignalMapper() const
	{
		return m_renderSignalMapper;
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
}