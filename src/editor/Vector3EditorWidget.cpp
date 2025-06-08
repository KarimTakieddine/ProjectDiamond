#include "Vector3EditorWidget.h"

namespace project_diamond
{
	Vector3EditorWidget::Vector3EditorWidget(QWidget* parent /* = nullptr */) :
		Vector2EditorWidget	(parent),
		m_zSpinBox			(new QDoubleSpinBox())
	{

	}

	void Vector3EditorWidget::setupUi()
	{
		Vector2EditorWidget::setupUi();
		m_layout->addWidget(m_zSpinBox);
	}

	void Vector3EditorWidget::connectUi()
	{
		Vector2EditorWidget::connectUi();
		connect(m_zSpinBox, &QDoubleSpinBox::valueChanged, this, &Vector3EditorWidget::onZChanged);
	}

	void Vector3EditorWidget::onXChanged(double x)
	{
		emit dataChanged(
			{
				static_cast<float>(x),
				static_cast<float>(m_ySpinBox->value()),
				static_cast<float>(m_zSpinBox->value())
			});
	}

	void Vector3EditorWidget::onYChanged(double y)
	{
		emit dataChanged(
			{
				static_cast<float>(m_xSpinBox->value()),
				static_cast<float>(y),
				static_cast<float>(m_zSpinBox->value())
			});
	}

	void Vector3EditorWidget::onZChanged(double z)
	{
		emit dataChanged(
			{
				static_cast<float>(m_xSpinBox->value()),
				static_cast<float>(m_ySpinBox->value()),
				static_cast<float>(z)
			});
	}

	void Vector3EditorWidget::setData(const glm::vec3& data)
	{
		Vector2EditorWidget::setData(data.xy);
		m_zSpinBox->setValue(data.z);
	}
}