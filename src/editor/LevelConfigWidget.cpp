#include <QFileDialog>
#include <QPushButton>

#include "LevelConfigListModel.h"
#include "LevelConfigTreeModel.h"
#include "LevelConfigWidget.h"
#include "ui_LevelConfigWidget.h"

namespace project_diamond
{
	LevelConfigWidget::LevelConfigWidget(QWidget* parent)
		: QWidget(parent)
		, m_ui(new Ui::LevelConfigWidget())
	{

	}

	void LevelConfigWidget::setupUi()
	{
		m_ui->setupUi(this);
		m_ui->levelsTableView->horizontalHeader()->setStretchLastSection(true);

		m_ui->removeCurrentButton->setEnabled(false);
		m_ui->clearAllButton->setEnabled(false);
	}

	void LevelConfigWidget::connectUi()
	{
		connect(m_ui->levelsTableView, &QTableView::doubleClicked, this, &LevelConfigWidget::onTableDoubleClicked);
		connect(m_ui->insertNewBeforeButton, &QPushButton::clicked, this, &LevelConfigWidget::onInsertNewBeforeClicked);
		connect(m_ui->insertNewAfterButton, &QPushButton::clicked, this, &LevelConfigWidget::onInsertNewAfterClicked);
		connect(m_ui->removeCurrentButton, &QPushButton::clicked, this, &LevelConfigWidget::onRemoveCurrentClicked);
		connect(m_ui->clearAllButton, &QPushButton::clicked, this, &LevelConfigWidget::onClearAllClicked);
	}

	void LevelConfigWidget::setListModel(LevelConfigListModel* model)
	{
		if (m_listModel)
		{
			disconnect(m_listModel, &LevelConfigListModel::dataChanged, this, &LevelConfigWidget::onLevelDataChanged);
			disconnect(m_listModel, &LevelConfigListModel::rowsRemoved, this, &LevelConfigWidget::onRowsRemoved);
			disconnect(m_listModel, &LevelConfigListModel::rowsInserted, this, &LevelConfigWidget::onRowsInserted);
		}

		QItemSelectionModel* previousSelectionModel = m_ui->levelsTableView->selectionModel();
		if (previousSelectionModel)
		{
			disconnect(previousSelectionModel, &QItemSelectionModel::selectionChanged, this, &LevelConfigWidget::onLevelSelectionChanged);
		}

		m_ui->levelsTableView->setModel(model);
		delete previousSelectionModel;

		connect(model, &LevelConfigListModel::dataChanged, this, &LevelConfigWidget::onLevelDataChanged);
		connect(model, &LevelConfigListModel::rowsRemoved, this, &LevelConfigWidget::onRowsRemoved);
		connect(model, &LevelConfigListModel::rowsInserted, this, &LevelConfigWidget::onRowsInserted);
		connect(m_ui->levelsTableView->selectionModel(), &QItemSelectionModel::selectionChanged, this, &LevelConfigWidget::onLevelSelectionChanged);

		m_listModel = model;
	}

	void LevelConfigWidget::setTreeModel(LevelConfigTreeModel* model)
	{
		if (m_treeModel)
		{
			// TODO
			disconnect(m_ui->levelConfigTreeView, &QTreeView::clicked, m_treeModel, &LevelConfigTreeModel::onIndexSelected);
			disconnect(m_treeModel, &LevelConfigTreeModel::renderConfigSelected, this, &LevelConfigWidget::onRenderConfigSelected);
			disconnect(m_treeModel, &LevelConfigTreeModel::behaviourConfigSelected, this, &LevelConfigWidget::onBehaviourConfigSelected);
		}

		QItemSelectionModel* previousSelectionModel = m_ui->levelConfigTreeView->selectionModel();
		if (previousSelectionModel)
		{
			// TODO
		}

		m_ui->levelConfigTreeView->setModel(model);
		delete previousSelectionModel;

		connect(m_ui->levelConfigTreeView, &QTreeView::clicked, model, &LevelConfigTreeModel::onIndexSelected);
		connect(model, &LevelConfigTreeModel::renderConfigSelected, this, &LevelConfigWidget::onRenderConfigSelected);
		connect(model, &LevelConfigTreeModel::behaviourConfigSelected, this, &LevelConfigWidget::onBehaviourConfigSelected);

		m_treeModel = model;
	}

	void LevelConfigWidget::onLevelSelectionChanged(const QItemSelection& selected)
	{
		if (!m_listModel)
		{
			return;
		}

		if (selected.size() == 0)
		{
			return;
		}

		QModelIndex modelIndex = m_listModel->index(selected.constLast().bottomRight().row(), 0);
		m_ui->removeCurrentButton->setEnabled(modelIndex.isValid());

		emit levelSelectionChanged(
			qvariant_cast<const LevelConfigModel*>(m_listModel->data(modelIndex, Qt::UserRole)));
	}

	void LevelConfigWidget::onLevelDataChanged(const QModelIndex& index)
	{
		emit levelDataChanged(
			qvariant_cast<const LevelConfigModel*>(m_listModel->data(index, Qt::UserRole)));
	}

	void LevelConfigWidget::onTableDoubleClicked(const QModelIndex& index)
	{
		if (!index.isValid() || index.column() != 1)
		{
			return;
		}

		const QString path = QFileDialog::getOpenFileName(
			nullptr,
			QStringLiteral("Select Level Config File (.xml)"),
			QStringLiteral("*.xml"));

		if (path.isNull())
		{
			return;
		}

		m_listModel->setData(index, path);
	}

	void LevelConfigWidget::onInsertNewBeforeClicked()
	{
		if (!m_listModel)
		{
			return;
		}

		const QModelIndex& currentIndex = m_ui->levelsTableView->currentIndex();
		m_listModel->insertRow(currentIndex.isValid() ? currentIndex.row() : 0);
	}

	void LevelConfigWidget::onInsertNewAfterClicked()
	{
		if (!m_listModel)
		{
			return;
		}

		const QModelIndex& currentIndex = m_ui->levelsTableView->currentIndex();
		m_listModel->insertRow(currentIndex.isValid() ? currentIndex.row() + 1 : 0);
	}

	void LevelConfigWidget::onRemoveCurrentClicked()
	{
		const QModelIndex& currentIndex = m_ui->levelsTableView->currentIndex();

		if (!currentIndex.isValid() || !m_listModel)
		{
			return;
		}

		m_listModel->removeRow(currentIndex.row());
	}

	void LevelConfigWidget::onClearAllClicked()
	{
		if (!m_listModel || m_listModel->rowCount() == 0)
		{
			return;
		}

		m_listModel->removeRows(0, m_listModel->rowCount());
	}

	void LevelConfigWidget::onRowsRemoved()
	{
		if (!m_listModel)
		{
			return;
		}

		const int rowCount = m_listModel->rowCount();
		m_ui->clearAllButton->setEnabled(rowCount != 0);

		if (rowCount == 0)
		{
			m_ui->removeCurrentButton->setEnabled(false);
			emit levelSelectionChanged(nullptr);
		}

		m_ui->levelsTableView->setFocus();
	}

	void LevelConfigWidget::onRowsInserted(const QModelIndex& parent, int first, int last)
	{
		Q_UNUSED(parent);
		Q_UNUSED(first);

		if (!m_listModel)
		{
			return;
		}

		QModelIndex lastIndex = m_listModel->index(last, 0);

		m_ui->clearAllButton->setEnabled(true);
		m_ui->levelsTableView->selectionModel()->select(lastIndex, QItemSelectionModel::ClearAndSelect);
		m_ui->levelsTableView->setCurrentIndex(lastIndex);

		m_ui->levelsTableView->setFocus();
	}

	void LevelConfigWidget::onRenderConfigSelected(int componentIndex, int instanceIndex)
	{
		const QModelIndex currentIndex = m_ui->levelsTableView->currentIndex();
		if (!currentIndex.isValid())
		{
			// TODO
			return;
		}

		auto* levelConfig = qvariant_cast<const LevelConfigModel*>(m_listModel->data(currentIndex, Qt::UserRole));
		if (!levelConfig)
		{
			// TODO
			return;
		}

		auto* levelData = levelConfig->getData();
		if (!levelData)
		{
			// TODO
			return;
		}

		const auto& instanceConfig = levelData->getInstanceConfigs().at(static_cast<size_t>(instanceIndex));
		if (!instanceConfig)
		{
			// TODO
			return;
		}

		const auto& renderComponent = instanceConfig->getRenderConfigs().at(static_cast<size_t>(componentIndex));
		if (!renderComponent)
		{
			// TODO
			return;
		}

		qDebug() << QString::fromStdString(renderComponent->getName());
	}

	void LevelConfigWidget::onBehaviourConfigSelected(int componentIndex, int instanceIndex)
	{
		const QModelIndex currentIndex = m_ui->levelsTableView->currentIndex();

		if (!currentIndex.isValid())
		{
			// TODO
			return;
		}

		
	}

	LevelConfigWidget::~LevelConfigWidget()
	{
		delete m_ui;
	}

	void LevelConfigWidget::saveCurrentLevel()
	{
		const QModelIndex& currentIndex = m_ui->levelsTableView->currentIndex();

		if (!currentIndex.isValid() || !m_listModel)
		{
			return;
		}

		auto* levelConfigData = qvariant_cast<LevelConfigModel*>(m_listModel->data(currentIndex, Qt::UserRole));
		if (!levelConfigData)
		{
			return;
		}

		if (levelConfigData->serialize(levelConfigData->getPath()))
		{
			emit levelSelectionChanged(levelConfigData);
		}
	}

	void LevelConfigWidget::saveCurrentLevelAs()
	{
		const QModelIndex& currentIndex = m_ui->levelsTableView->currentIndex();

		if (!currentIndex.isValid() || !m_listModel)
		{
			return;
		}

		auto* levelConfigData = qvariant_cast<LevelConfigModel*>(m_listModel->data(currentIndex, Qt::UserRole));
		if (!levelConfigData)
		{
			return;
		}

		const QString file = QFileDialog::getSaveFileName(
			nullptr,
			QStringLiteral("Save Level As..."),
			QDir::currentPath(),
			"*.xml");

		if (file.isNull())
		{
			return;
		}

		if (levelConfigData->serialize(file))
		{
			emit levelSelectionChanged(levelConfigData);
		}
	}
}