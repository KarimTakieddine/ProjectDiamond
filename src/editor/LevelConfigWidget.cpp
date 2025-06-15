#include <QFileDialog>
#include <QPushButton>

#include "ComponentEditorWidget.h"
#include "LevelConfigListModel.h"
#include "LevelConfigTreeContainer.h"
#include "LevelConfigWidget.h"
#include "TextureModel.h"
#include "ui_LevelConfigWidget.h"

namespace project_diamond
{
	LevelConfigWidget::LevelConfigWidget(QWidget* parent)
		: QWidget(parent)
		, m_ui				(new Ui::LevelConfigWidget())
		, m_componentEditor	(new ComponentEditorWidget())
		, m_listModel		(new LevelConfigListModel(this))
		, m_treeContainer	(new LevelConfigTreeContainer(this))
	{

	}

	void LevelConfigWidget::setupUi()
	{
		m_ui->setupUi(this);
		m_ui->levelsTableView->horizontalHeader()->setStretchLastSection(true);

		m_ui->removeCurrentButton->setEnabled(false);
		m_ui->clearAllButton->setEnabled(false);

		m_componentEditor->setupUi();
		m_ui->componentConfigLayout->addWidget(m_componentEditor);
		m_componentEditor->setEnabled(false);
	}

	void LevelConfigWidget::connectUi()
	{
		m_componentEditor->connectUi();

		m_ui->levelsTableView->setModel(m_listModel);

		connect(m_listModel, &LevelConfigListModel::dataChanged,	this, &LevelConfigWidget::onLevelDataChanged);
		connect(m_listModel, &LevelConfigListModel::rowsRemoved,	this, &LevelConfigWidget::onRowsRemoved);
		connect(m_listModel, &LevelConfigListModel::rowsInserted,	this, &LevelConfigWidget::onRowsInserted);

		connect(m_ui->levelsTableView->selectionModel(), &QItemSelectionModel::selectionChanged, this, &LevelConfigWidget::onLevelSelectionChanged);

		connect(m_ui->levelsTableView, &QTableView::doubleClicked,	this, &LevelConfigWidget::onTableDoubleClicked);
		connect(m_ui->insertNewBeforeButton, &QPushButton::clicked, this, &LevelConfigWidget::onInsertNewBeforeClicked);
		connect(m_ui->insertNewAfterButton, &QPushButton::clicked,	this, &LevelConfigWidget::onInsertNewAfterClicked);
		connect(m_ui->removeCurrentButton, &QPushButton::clicked,	this, &LevelConfigWidget::onRemoveCurrentClicked);
		connect(m_ui->clearAllButton, &QPushButton::clicked,		this, &LevelConfigWidget::onClearAllClicked);
	}

	void LevelConfigWidget::setTextureModel(TextureModel* model)
	{
		if (m_textureModel)
		{
			// TODO

			disconnect(m_textureModel, &TextureModel::imageChanged, m_componentEditor, &ComponentEditorWidget::imageChanged);
		}

		// TODO

		connect(model, &TextureModel::imageChanged, m_componentEditor, &ComponentEditorWidget::imageChanged);

		m_componentEditor->setTextureModel(model);

		m_textureModel = model;
	}

	void LevelConfigWidget::onLevelSelectionChanged(const QItemSelection& selected)
	{
		m_componentEditor->clear();
		m_componentEditor->setEnabled(false);

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

		auto* configModel = qvariant_cast<const LevelConfigModel*>(m_listModel->data(modelIndex, Qt::UserRole));
		if (!configModel)
		{
			// TODO
			return;
		}

		QTreeView* const treeView = m_ui->levelConfigTreeView;

		QAbstractItemModel* const previousTreeModel = treeView->model();
		if (previousTreeModel)
		{
			disconnect(treeView, &QTreeView::clicked, dynamic_cast<LevelConfigTreeModel*>(previousTreeModel), &LevelConfigTreeModel::onIndexSelected); // TODO: More error handling here
		}

		LevelConfigTreeModel* treeModel = m_treeContainer->getTreeModel(configModel->getUuid());
		if (treeModel)
		{
			QItemSelectionModel* previousSelectionModel = treeView->selectionModel();
			treeView->setModel(treeModel);
			delete previousSelectionModel;
			treeView->update();

			connect(treeView, &QTreeView::clicked, treeModel, &LevelConfigTreeModel::onIndexSelected);
		}
		else
		{
			// TODO
		}

		emit levelSelectionChanged(configModel);
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

	// TODO: On rows about to be removed - remove entry in tree map for that UUID

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

		if (!m_listModel)
		{
			return;
		}

		for (int i = first; i <= last; ++i)
		{
			auto* configModel = qvariant_cast<const LevelConfigModel*>(m_listModel->data(m_listModel->index(i, 0), Qt::UserRole));
			if (!configModel)
			{
				// TODO
				continue;
			}

			LevelConfigTreeModel* treeModel = m_treeContainer->insertTreeModel(configModel->getUuid());
			if (!treeModel)
			{
				// TODO
				continue;
			}

			connect(treeModel, &LevelConfigTreeModel::renderConfigSelected,		this, &LevelConfigWidget::onRenderConfigSelected);
			connect(treeModel, &LevelConfigTreeModel::behaviourConfigSelected,	this, &LevelConfigWidget::onBehaviourConfigSelected);

			connect(configModel, &LevelConfigModel::pathChanged,				treeModel, &LevelConfigTreeModel::clear);
			connect(configModel, &LevelConfigModel::gameInstanceInserted,		treeModel, &LevelConfigTreeModel::onGameInstanceInserted);
			connect(configModel, &LevelConfigModel::gameInstanceRemoved,		treeModel, &LevelConfigTreeModel::onGameInstanceRemoved);
			connect(configModel, &LevelConfigModel::renderComponentInserted,	treeModel, &LevelConfigTreeModel::onRenderComponentInserted);
			connect(configModel, &LevelConfigModel::renderComponentRemoved,		treeModel, &LevelConfigTreeModel::onRenderComponentRemoved);
		}

		QModelIndex lastIndex = m_listModel->index(last, 0);

		m_ui->clearAllButton->setEnabled(true);
		m_ui->levelsTableView->selectionModel()->select(lastIndex, QItemSelectionModel::ClearAndSelect);
		m_ui->levelsTableView->setCurrentIndex(lastIndex);

		m_ui->levelsTableView->setFocus();
	}

	void LevelConfigWidget::onRenderConfigSelected(int componentIndex, int instanceIndex)
	{
		if (componentIndex < 0 || instanceIndex < 0)
		{
			m_componentEditor->clear();
			m_componentEditor->setEnabled(false);
			return;
		}

		const QModelIndex currentIndex = m_ui->levelsTableView->currentIndex();
		if (!currentIndex.isValid())
		{
			m_componentEditor->clear();
			m_componentEditor->setEnabled(false);
			return;
		}

		auto* levelConfig = qvariant_cast<const LevelConfigModel*>(m_listModel->data(currentIndex, Qt::UserRole));
		if (!levelConfig)
		{
			m_componentEditor->clear();
			m_componentEditor->setEnabled(false);
			return;
		}

		const auto& gameInstances = levelConfig->getInstances();
		if (instanceIndex >= gameInstances.count())
		{
			m_componentEditor->clear();
			m_componentEditor->setEnabled(false);
			return;
		}

		const auto& gameInstance = gameInstances.at(instanceIndex);
		if (!gameInstance)
		{
			m_componentEditor->clear();
			m_componentEditor->setEnabled(false);
			return;
		}

		const auto& renderComponents = gameInstance->getRenderComponents();
		if (componentIndex >= renderComponents.size())
		{
			m_componentEditor->clear();
			m_componentEditor->setEnabled(false);
			return;
		}

		const auto& renderComponent = renderComponents.at(componentIndex);
		if (!renderComponent)
		{
			m_componentEditor->clear();
			m_componentEditor->setEnabled(false);
			return;
		}

		m_componentEditor->setEnabled(
			m_componentEditor->configureComponent(renderComponent.get(), renderComponent->getData()->getName()));
	}

	void LevelConfigWidget::onBehaviourConfigSelected(int componentIndex, int instanceIndex)
	{
		const QModelIndex currentIndex = m_ui->levelsTableView->currentIndex();
		if (!currentIndex.isValid())
		{
			// TODO
			return;
		}

		// TODO
	}

	LevelConfigWidget::~LevelConfigWidget()
	{
		delete m_ui;
	}

	void LevelConfigWidget::loadLevels()
	{
		const QString directory = QFileDialog::getExistingDirectory(
			nullptr,
			QStringLiteral("Load Levels from Folder"));

		if (directory.isNull())
		{
			return;
		}

		m_listModel->loadLevels(directory);
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