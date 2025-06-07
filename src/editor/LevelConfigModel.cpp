#include <pugixml.hpp>

#include <glm/vec4.hpp>

#include <parser/GameSceneConfigParser.h>

#include "LevelConfigModel.h"

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
}

namespace project_diamond
{
	LevelConfigModel::LevelConfigModel(QObject* parent /* = nullptr */) :
		QObject	(parent),
		m_data	(std::make_unique<diamond_engine::GameSceneConfig>()),
		m_name	(QString::fromStdString(m_data->getName())) { }

	diamond_engine::GameSceneConfig* LevelConfigModel::getData() const
	{
		return m_data.get();
	}

	bool LevelConfigModel::setPath(const QString& path)
	{
		if (path == m_path)
		{
			return false;
		}

		diamond_engine::EngineStatus status;
		auto data = diamond_engine::parseSceneFile(path.toStdString(), &status);
		emit parseStatus(QString::fromStdString(status.message));

		if (!data)
		{
			return false;
		}

		m_color = ::vec4ToColor(data->getBackgroundColor());
		m_name	= QString::fromStdString(data->getName());
		m_path	= path;

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

	const QString LevelConfigModel::getName() const
	{
		return m_name;
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

		if (!document.save_file(path.c_str(), "\t", pugi::format_indent, pugi::encoding_utf8))
		{
			emit parseStatus(QStringLiteral("Failed to serialise level: ") + m_name + QStringLiteral(" to: ") + file);
			return false;
		}

		m_path = file;
		setDirty(false);
		return true;
	}
}