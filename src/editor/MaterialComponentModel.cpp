#include <config/MaterialComponentConfig.h>

#include "MaterialComponentModel.h"

namespace project_diamond
{
	MaterialComponentModel::MaterialComponentModel(QObject* parent /* = nullptr */) : RenderComponentModel(parent) {}

	void MaterialComponentModel::setData(diamond_engine::RenderComponentConfig* data)
	{
		m_data = dynamic_cast<diamond_engine::MaterialComponentConfig*>(data);
	}

	const diamond_engine::RenderComponentConfig* MaterialComponentModel::getData() const
	{
		return m_data;
	}

	const glm::vec3& MaterialComponentModel::getColor() const
	{
		return m_data->getColor();
	}

	const glm::vec2& MaterialComponentModel::getTextureOffset() const
	{
		return m_data->getTextureOffset();
	}

	void MaterialComponentModel::setColor(const glm::vec3& color)
	{
		if (!m_data)
		{
			return;
		}

		if (color == m_data->getColor())
		{
			return;
		}

		m_data->setColor(color);

		emit colorChanged(color);
	}

	void MaterialComponentModel::setTextureOffset(const glm::vec2& textureOffset)
	{
		if (!m_data)
		{
			return;
		}

		if (textureOffset == m_data->getTextureOffset())
		{
			return;
		}

		m_data->setTextureOffset(textureOffset);

		emit textureOffsetChanged(textureOffset);
	}

	void MaterialComponentModel::setTextureName(const QString& textureName)
	{
		if (!m_data)
		{
			return;
		}

		if (textureName == QString::fromStdString(m_data->getTextureName()))
		{
			return;
		}

		m_data->setTextureName(textureName.toStdString());

		emit textureNameChanged(textureName);
	}
}