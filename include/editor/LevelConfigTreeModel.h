#pragma once

#include <functional>

#include <QHash>
#include <QStandardItemModel>

namespace project_diamond
{
	class LevelConfigModel;
	class LevelConfigTreeModel : public QStandardItemModel
	{
		Q_OBJECT

	public:
		using EmitSignalFunc = std::function<void(LevelConfigTreeModel*, int, int)>;

		LevelConfigTreeModel(QObject* parent = nullptr);

	signals:
		void renderConfigSelected(int componentIndex, int instanceIndex);
		void behaviourConfigSelected(int componentIndex, int instanceIndex);

	public slots:
		void onGameInstanceInserted(int index, const QString& name);
		void onGameInstanceRemoved(int index);
		void onRenderComponentInserted(int instanceIndex, int componentIndex, const QString& name);
		void onRenderComponentRemoved(int instanceIndex, int componentIndex);

		void onIndexSelected(const QModelIndex& selectedIndex);

	private:
		static const QHash<QString, EmitSignalFunc> signalsToEmit;
	};
}
