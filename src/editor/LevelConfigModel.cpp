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
	LevelConfigModel::LevelConfigModel(QObject* parent /* = nullptr */) : QObject(parent)
	{
		connect(this, &LevelConfigModel::pathChanged, this, &LevelConfigModel::onPathChanged);
		connect(this, &LevelConfigModel::colorChanged, this, &LevelConfigModel::onColorChanged);
	}

	diamond_engine::GameSceneConfig* LevelConfigModel::getData() const
	{
		return m_data.get();
	}

	void LevelConfigModel::setData(std::unique_ptr<diamond_engine::GameSceneConfig> data)
	{
		if (data == m_data)
		{
			return;
		}

		m_data = std::move(data);

		if (m_data)
		{
			m_color = ::vec4ToColor(m_data->getBackgroundColor());
		}

		emit dataChanged();
	}

	void LevelConfigModel::setPath(const QString& path)
	{
		if (path == m_path)
		{
			return;
		}

		m_path = path;

		emit pathChanged(path);
	}

	const QString& LevelConfigModel::getPath() const
	{
		return m_path;
	}

	void LevelConfigModel::onPathChanged(const QString& path)
	{
		diamond_engine::EngineStatus status;
		setData(diamond_engine::parseSceneFile(path.toStdString(), &status));
		emit parseStatus(QString::fromStdString(status.message));
	}

	void LevelConfigModel::onColorChanged(const QColor& color)
	{
		if (!m_data)
		{
			return;
		}

		m_data->setBackgroundColor(::colorToVec4(color));

		emit dataChanged();
	}

	void LevelConfigModel::setName(const QString& name)
	{
		if (name == getName())
		{
			return;
		}

		if (!m_data)
		{
			return;
		}

		m_data->setName(name.toStdString());

		emit nameChanged(name);
	}

	const QString LevelConfigModel::getName() const
	{
		return m_data ? QString::fromStdString(m_data->getName()) : QStringLiteral("null");
	}

	void LevelConfigModel::setColor(const QColor& color)
	{
		if (color == m_color)
		{
			return;
		}

		m_color = color;

		emit colorChanged(color);
	}

	const QColor& LevelConfigModel::getColor() const
	{
		return m_color;
	}
}