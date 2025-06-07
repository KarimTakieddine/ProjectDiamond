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
	LevelConfigModel::LevelConfigModel(QObject* parent /* = nullptr */) : QObject(parent) { }

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
		m_data = diamond_engine::parseSceneFile(path.toStdString(), &status);
		emit parseStatus(QString::fromStdString(status.message));

		if (m_data)
		{
			m_color = ::vec4ToColor(m_data->getBackgroundColor());
		}

		m_path = path;
		
		return true;
	}

	const QString& LevelConfigModel::getPath() const
	{
		return m_path;
	}

	bool LevelConfigModel::setName(const QString& name)
	{
		if (name == getName())
		{
			return false;
		}

		if (!m_data)
		{
			return false;
		}

		m_data->setName(name.toStdString());

		return true;
	}

	const QString LevelConfigModel::getName() const
	{
		return m_data ? QString::fromStdString(m_data->getName()) : QStringLiteral("null");
	}

	bool LevelConfigModel::setColor(const QColor& color)
	{
		if (color == m_color)
		{
			return false;
		}

		if (!m_data)
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
}