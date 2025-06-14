#include "ComponentEditorWidget.h"
#include "TransformComponentModel.h"
#include "Vector2EditorWidget.h"
#include "Vector3EditorWidget.h"
#include "Vector4EditorWidget.h"

namespace
{
	template<typename T>
	T* createComponentWidget()
	{
		auto* result = new T();

		result->setupUi();
		result->connectUi();
		result->setAttribute(Qt::WA_DeleteOnClose);

		return result;
	}
}

namespace project_diamond
{
	ComponentEditorWidget::ComponentEditorWidget(QWidget* parent) : QWidget(parent), m_layout(new QVBoxLayout())
	{
		m_configureFunctions =
		{
			{ QStringLiteral("Transform"), std::bind(&ComponentEditorWidget::configureTransformComponent, this, std::placeholders::_1) }
		};
	}

	void ComponentEditorWidget::setupUi()
	{
		m_layout->addSpacerItem(new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::MinimumExpanding));

		setLayout(m_layout);
	}

	void ComponentEditorWidget::connectUi()
	{

	}

	void ComponentEditorWidget::clear()
	{
		for (auto w : m_childWidgets)
		{
			m_layout->removeWidget(w);
			w->close();
		}

		m_childWidgets.clear();
	}

	bool ComponentEditorWidget::configureComponent(RenderComponentModel* component, const QString& name)
	{
		clear();

		auto configureIt = m_configureFunctions.constFind(name);
		if (configureIt == m_configureFunctions.cend())
		{
			return false;
		}

		return (*configureIt)(component);
	}

	bool ComponentEditorWidget::configureTransformComponent(RenderComponentModel* component)
	{
		auto* model = dynamic_cast<TransformComponentModel*>(component);
		if (!model)
		{
			return false;
		}

		auto* positionEditorWidget = ::createComponentWidget<Vector3EditorWidget>();
		positionEditorWidget->setTitle(QStringLiteral("Position"));
		positionEditorWidget->setMinimum({ -10000.0f, -10000.0f, -10000.0f });
		positionEditorWidget->setMaximum({ 10000.0f, 10000.0f, 10000.0f });
		positionEditorWidget->setData(model->getPosition());
		connect(positionEditorWidget, &Vector3EditorWidget::dataChanged, model, &TransformComponentModel::setPosition);
		insertWidget(positionEditorWidget, 0);

		auto* localScaleEditorWidget = ::createComponentWidget<Vector3EditorWidget>();
		localScaleEditorWidget->setTitle(QStringLiteral("Local Scale"));
		localScaleEditorWidget->setMinimum({ 0.0f, 0.0f, 0.0f });
		localScaleEditorWidget->setMaximum({ 10000.0f, 10000.0f, 10000.0f });
		localScaleEditorWidget->setData(model->getLocalScale());
		connect(localScaleEditorWidget, &Vector3EditorWidget::dataChanged, model, &TransformComponentModel::setLocalScale);
		insertWidget(localScaleEditorWidget, 1);

		m_layout->setStretch(m_layout->count() - 1, 1);

		return true;
	}

	void ComponentEditorWidget::insertWidget(QWidget* widget, int index)
	{
		m_layout->insertWidget(index, widget);
		m_childWidgets.insert(index, widget);
	}
}