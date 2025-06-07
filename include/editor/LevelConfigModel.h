#pragma once

#include <memory>

#include <QColor>
#include <QObject>

#include <config/GameSceneConfig.h>

namespace project_diamond
{
	class LevelConfigModel : public QObject
	{
		Q_OBJECT

	public:
		LevelConfigModel(QObject* parent = nullptr);

		diamond_engine::GameSceneConfig* getData() const;

		bool setPath(const QString& path);
		const QString& getPath() const;

		bool setName(const QString& name);
		const QString getName() const;

		bool setColor(const QColor& color);
		const QColor& getColor() const;

	signals:
		void parseStatus(const QString&);

	private:
		std::unique_ptr<diamond_engine::GameSceneConfig> m_data{ nullptr };
		QString m_name{ };
		QString m_path{ };
		QColor m_color{ };
	};
}
