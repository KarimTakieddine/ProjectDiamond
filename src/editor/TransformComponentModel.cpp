#include <config/TransformComponentConfig.h>

#include "TransformComponentModel.h"

namespace project_diamond
{
	TransformComponentModel::TransformComponentModel(QObject* parent /* = nullptr */) : RenderComponentModel(parent) {}

	void TransformComponentModel::setData(diamond_engine::RenderComponentConfig* data)
	{
		m_data = dynamic_cast<diamond_engine::TransformComponentConfig*>(data);
	}

	const diamond_engine::RenderComponentConfig* TransformComponentModel::getData() const
	{
		return m_data;
	}

	const glm::vec3& TransformComponentModel::getPosition() const
	{
		return m_data->getPosition();
	}

	const glm::vec3& TransformComponentModel::getLocalScale() const
	{
		return m_data->getLocalScale();
	}

	void TransformComponentModel::setPosition(const glm::vec3& position)
	{
		if (!m_data)
		{
			return;
		}

		if (position == m_data->getPosition())
		{
			return;
		}

		m_data->setPosition(position);

		emit positionChanged(position);
	}

	void TransformComponentModel::setLocalScale(const glm::vec3& localScale)
	{
		if (!m_data)
		{
			return;
		}

		if (localScale == m_data->getLocalScale())
		{
			return;
		}

		m_data->setLocalScale(localScale);

		emit localScaleChanged(localScale);
	}
}