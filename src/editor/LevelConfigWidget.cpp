#include <QFileDialog>
#include <QPushButton>

#include "LevelConfigListModel.h"
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

	void LevelConfigWidget::setModel(LevelConfigListModel* model)
	{
		if (m_model)
		{
			disconnect(m_model, &LevelConfigListModel::dataChanged, this, &LevelConfigWidget::onLevelDataChanged);
			disconnect(m_model, &LevelConfigListModel::rowsRemoved, this, &LevelConfigWidget::onRowsRemoved);
			disconnect(m_model, &LevelConfigListModel::rowsInserted, this, &LevelConfigWidget::onRowsInserted);
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

		m_model = model;
	}

	void LevelConfigWidget::onLevelSelectionChanged(const QItemSelection& selected)
	{
		if (!m_model)
		{
			return;
		}

		if (selected.size() == 0)
		{
			return;
		}

		QModelIndex modelIndex = m_model->index(selected.constLast().bottomRight().row(), 0);
		m_ui->removeCurrentButton->setEnabled(modelIndex.isValid());

		emit levelSelectionChanged(
			qvariant_cast<LevelConfigModel*>(m_model->data(modelIndex, Qt::UserRole)));
	}

	void LevelConfigWidget::onLevelDataChanged(const QModelIndex& index)
	{
		emit levelDataChanged(
			qvariant_cast<LevelConfigModel*>(m_model->data(index, Qt::UserRole)));
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

		m_model->setData(index, path);
	}

	void LevelConfigWidget::onInsertNewBeforeClicked()
	{
		if (!m_model)
		{
			return;
		}

		const QModelIndex& currentIndex = m_ui->levelsTableView->currentIndex();
		m_model->insertRow(currentIndex.isValid() ? currentIndex.row() : 0);
	}

	void LevelConfigWidget::onInsertNewAfterClicked()
	{
		if (!m_model)
		{
			return;
		}

		const QModelIndex& currentIndex = m_ui->levelsTableView->currentIndex();
		m_model->insertRow(currentIndex.isValid() ? currentIndex.row() + 1 : 0);
	}

	void LevelConfigWidget::onRemoveCurrentClicked()
	{
		const QModelIndex& currentIndex = m_ui->levelsTableView->currentIndex();

		if (!currentIndex.isValid() || !m_model)
		{
			return;
		}

		m_model->removeRow(currentIndex.row());
	}

	void LevelConfigWidget::onClearAllClicked()
	{
		if (!m_model || m_model->rowCount() == 0)
		{
			return;
		}

		m_model->removeRows(0, m_model->rowCount());
	}

	void LevelConfigWidget::onRowsRemoved()
	{
		if (!m_model)
		{
			return;
		}

		const int rowCount = m_model->rowCount();
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

		if (!m_model)
		{
			return;
		}

		QModelIndex lastIndex = m_model->index(last, 0);

		m_ui->clearAllButton->setEnabled(true);
		m_ui->levelsTableView->selectionModel()->select(lastIndex, QItemSelectionModel::ClearAndSelect);
		m_ui->levelsTableView->setCurrentIndex(lastIndex);

		m_ui->levelsTableView->setFocus();
	}

	LevelConfigWidget::~LevelConfigWidget()
	{
		delete m_ui;
	}

	void LevelConfigWidget::saveCurrentLevel()
	{
		const QModelIndex& currentIndex = m_ui->levelsTableView->currentIndex();

		if (!currentIndex.isValid() || !m_model)
		{
			return;
		}

		auto* levelConfigData = qvariant_cast<LevelConfigModel*>(m_model->data(currentIndex, Qt::UserRole));
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

		if (!currentIndex.isValid() || !m_model)
		{
			return;
		}

		auto* levelConfigData = qvariant_cast<LevelConfigModel*>(m_model->data(currentIndex, Qt::UserRole));
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