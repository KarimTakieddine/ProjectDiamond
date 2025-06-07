#include <QFileDialog>

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
	}

	void LevelConfigWidget::connectUi()
	{
		connect(m_ui->levelsTableView, &QTableView::doubleClicked, this, &LevelConfigWidget::onTableDoubleClicked);
		connect(m_ui->insertNewButton, &QPushButton::clicked, this, &LevelConfigWidget::onInsertNewClicked);
		connect(m_ui->removeCurrentButton, &QPushButton::clicked, this, &LevelConfigWidget::onRemoveCurrentClicked);
		connect(m_ui->clearAllButton, &QPushButton::clicked, this, &LevelConfigWidget::onClearAllClicked);
	}

	void LevelConfigWidget::setModel(LevelConfigListModel* model)
	{
		if (m_model)
		{
			disconnect(m_model, &LevelConfigListModel::dataChanged, this, &LevelConfigWidget::onLevelDataChanged);
		}

		QItemSelectionModel* previousSelectionModel = m_ui->levelsTableView->selectionModel();
		if (previousSelectionModel)
		{
			disconnect(previousSelectionModel, &QItemSelectionModel::selectionChanged, this, &LevelConfigWidget::onLevelSelectionChanged);
		}

		m_ui->levelsTableView->setModel(model);
		delete previousSelectionModel;

		connect(model, &LevelConfigListModel::dataChanged, this, &LevelConfigWidget::onLevelDataChanged);
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

		const int row = selected.constLast().bottomRight().row();

		emit levelSelectionChanged(
			qvariant_cast<LevelConfigModel*>(m_model->data(m_model->index(row, 0), Qt::UserRole)));
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

		m_model->setData(index, path);
	}

	void LevelConfigWidget::onInsertNewClicked()
	{
		if (!m_model)
		{
			return;
		}

		const QModelIndex& currentIndex = m_ui->levelsTableView->currentIndex();
		m_model->insertRow(currentIndex.isValid() ? currentIndex.row() : 0);
	}

	void LevelConfigWidget::onRemoveCurrentClicked()
	{

	}

	void LevelConfigWidget::onClearAllClicked()
	{

	}

	LevelConfigWidget::~LevelConfigWidget()
	{
		delete m_ui;
	}
}