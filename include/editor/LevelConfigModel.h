#pragma once

#include <memory>

#include <QColor>
#include <QObject>

#include <config/GameSceneConfig.h>

#include "GameInstanceModel.h"

class QSignalMapper;
namespace project_diamond
{
	class LevelConfigModel : public QObject
	{
		Q_OBJECT

	public:
		using RComponentCreateFunc = std::function<QSharedPointer<RenderComponentModel>(void)>;

		LevelConfigModel(QObject* parent = nullptr);

		const diamond_engine::GameSceneConfig* getData() const;

		bool setPath(const QString& path);
		const QString& getPath() const;

		bool setName(const QString& name);
		const QString getName() const;

		bool setColor(const QColor& color);
		const QColor& getColor() const;

		bool isDirty() const;
		void setDirty(bool dirty);

		bool serialize(const QString& file);

		const QVector<QSharedPointer<GameInstanceModel>>& getInstances() const;
		void insertGameInstance(qsizetype index, const QSharedPointer<GameInstanceModel>& instance);
		void removeGameInstance(qsizetype index);

	signals:
		void parseStatus(const QString&);
		void gameInstanceInserted(qsizetype index);
		void gameInstanceRemoved(qsizetype index);
		void instanceDataChanged(int);

	private:
		QVector<QSharedPointer<GameInstanceModel>> m_instances;
		std::unique_ptr<diamond_engine::GameSceneConfig> m_data	{ nullptr };
		QSignalMapper* m_signalMapper							{ nullptr };
		QString m_name											{ };
		QString m_path											{ };
		QColor m_color											{ };
		bool m_dirty											{ true };
	};
}
