#pragma once

#include <functional>

#include <QHash>
#include <QObject>
#include <QSharedPointer>
#include <QString>
#include <QVector>

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

	signals:
		void renderComponentInserted(qsizetype, const QString&);
		void renderComponentRemoved(qsizetype, const QString&);
		void renderComponentDataChanged(int);
		void behaviourComponentDataChanged(int);

	private:
		QVector<QSharedPointer<RenderComponentModel>> m_renderComponents;
		QSignalMapper* m_renderSignalMapper{ nullptr };
		QSignalMapper* m_behaviourSignalMapper{ nullptr };
	};
}
