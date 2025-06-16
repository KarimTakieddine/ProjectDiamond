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

	bool TransformComponentModel::unparse(pugi::xml_node& node) const
	{
		if (!m_data)
		{
			return false;
		}

		if (!node.set_name(m_data->getName()))
		{
			return false;
		}
		
		pugi::xml_node positionNode = node.append_child("Position");
		if (!positionNode)
		{
			return false;
		}
		const glm::vec3& position = getPosition();

		positionNode.append_attribute("x").set_value(position.x);
		positionNode.append_attribute("y").set_value(position.y);
		positionNode.append_attribute("z").set_value(position.z);

		pugi::xml_node localScaleNode = node.append_child("LocalScale");
		if (!localScaleNode)
		{
			return false;
		}
		const glm::vec3& localScale = getLocalScale();

		localScaleNode.append_attribute("x").set_value(localScale.x);
		localScaleNode.append_attribute("y").set_value(localScale.y);
		localScaleNode.append_attribute("z").set_value(localScale.z);

		return true;
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