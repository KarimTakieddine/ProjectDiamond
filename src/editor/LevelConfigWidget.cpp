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

	void LevelConfigWidget::setModel(LevelConfigListModel* model)
	{
		QItemSelectionModel* previousSelectionModel = m_ui->levelsTableView->selectionModel();
		if (previousSelectionModel)
		{
			disconnect(previousSelectionModel, &QItemSelectionModel::selectionChanged, this, &LevelConfigWidget::onLevelSelectionChanged);
		}

		m_ui->levelsTableView->setModel(model);
		delete previousSelectionModel;

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

	LevelConfigWidget::~LevelConfigWidget()
	{
		delete m_ui;
	}
}