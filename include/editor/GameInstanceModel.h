#pragma once

#include <functional>

#include <QHash>
#include <QObject>
#include <QSharedPointer>
#include <QString>
#include <QVector>

#include "EngineMetaTypeRegistry.h"
#include "RenderComponentModel.h"

class QSignalMapper;

namespace project_diamond
{
	class GameInstanceModel : public QObject
	{
		Q_OBJECT

	public:
		using RComponentConnectFunc = std::function<void(GameInstanceModel*, const QSharedPointer<RenderComponentModel>&, qsizetype)>;

		GameInstanceModel(QObject* parent = nullptr);

		QSignalMapper* getRenderSignalMapper() const;

		void insertRenderComponent(qsizetype index, const QSharedPointer<RenderComponentModel>& component);
		void removeRenderComponent(qsizetype index);
		const QVector<QSharedPointer<RenderComponentModel>>& getRenderComponents() const;
		bool unparse(pugi::xml_node& node) const;

		diamond_engine::GameInstanceType getType() const;
		const QString& getName() const;

	public slots:
		void setType(diamond_engine::GameInstanceType type);
		void setName(const QString& name);

	signals:
		void renderComponentInserted(qsizetype, const QString&);
		void renderComponentRemoved(qsizetype);
		void renderComponentDataChanged(int);
		void behaviourComponentDataChanged(int);
		void instanceTypeChanged(diamond_engine::GameInstanceType type);
		void nameChanged(const QString& name);

	private:
		QVector<QSharedPointer<RenderComponentModel>> m_renderComponents;
		QString m_name							{ QStringLiteral("Unnamed") };
		QSignalMapper* m_renderSignalMapper		{ nullptr };
		QSignalMapper* m_behaviourSignalMapper	{ nullptr };
		diamond_engine::GameInstanceType m_type	{ diamond_engine::GameInstanceType::UNKNOWN };
	};
}
