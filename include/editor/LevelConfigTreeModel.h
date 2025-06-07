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

		void loadLevelConfig(const LevelConfigModel* levelConfig);

	signals:
		void renderConfigSelected(int componentIndex, int instanceIndex);
		void behaviourConfigSelected(int componentIndex, int instanceIndex);

	public slots:
		void onIndexSelected(const QModelIndex& selectedIndex);

	private:
		static const QHash<QString, EmitSignalFunc> signalsToEmit;
	};
}
