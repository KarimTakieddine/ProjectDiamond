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

		void setData(std::unique_ptr<diamond_engine::GameSceneConfig> data);
		void setPath(const QString& path);
		const QString& getPath() const;

		void setName(const QString& name);
		const QString getName() const;

		void setColor(const QColor& color);
		const QColor& getColor() const;

	signals:
		void dataChanged();
		void pathChanged(const QString&);
		void nameChanged(const QString&);
		void colorChanged(const QColor&);
		void parseStatus(const QString&);

	public slots:
		void onPathChanged(const QString& path);
		void onColorChanged(const QColor& color);

	private:
		std::unique_ptr<diamond_engine::GameSceneConfig> m_data{ nullptr };
		QString m_path{ };
		QColor m_color{ };
	};
}
