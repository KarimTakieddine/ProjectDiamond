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

	bool MaterialComponentModel::unparse(pugi::xml_node& node) const
	{
		if (!m_data)
		{
			return false;
		}

		if (!node.set_name(m_data->getName()))
		{
			return false;
		}

		node.append_attribute("texture").set_value(m_data->getTextureName().c_str());

		pugi::xml_node colorNode = node.append_child("Color");
		if (!colorNode)
		{
			return false;
		}

		const glm::vec3& color = getColor();
		colorNode.append_attribute("r").set_value(color.r);
		colorNode.append_attribute("r").set_value(color.g);
		colorNode.append_attribute("r").set_value(color.b);

		pugi::xml_node textureOffsetNode = node.append_child("TextureOffset");
		if (!textureOffsetNode)
		{
			return false;
		}

		const glm::vec2& textureOffset = getTextureOffset();
		textureOffsetNode.append_attribute("x").set_value(textureOffset.x);
		textureOffsetNode.append_attribute("y").set_value(textureOffset.y);

		return true;
	}

	const glm::vec3& MaterialComponentModel::getColor() const
	{
		return m_data->getColor();
	}

	const glm::vec2& MaterialComponentModel::getTextureOffset() const
	{
		return m_data->getTextureOffset();
	}

	QString MaterialComponentModel::getTextureName() const
	{
		return QString::fromStdString(m_data->getTextureName());
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