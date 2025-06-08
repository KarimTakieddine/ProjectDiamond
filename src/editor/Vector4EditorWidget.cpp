#include "Vector4EditorWidget.h"

namespace project_diamond
{
	Vector4EditorWidget::Vector4EditorWidget(QWidget* parent /* = nullptr */) :
		Vector3EditorWidget	(parent),
		m_wSpinBox			(new QDoubleSpinBox())
	{

	}

	void Vector4EditorWidget::setupUi()
	{
		Vector3EditorWidget::setupUi();
		m_layout->addWidget(m_wSpinBox);
	}

	void Vector4EditorWidget::connectUi()
	{
		Vector3EditorWidget::connectUi();
		connect(m_wSpinBox, &QDoubleSpinBox::valueChanged, this, &Vector4EditorWidget::onWChanged);
	}

	void Vector4EditorWidget::onXChanged(double x)
	{
		emit dataChanged(
			{
				static_cast<float>(x),
				static_cast<float>(m_ySpinBox->value()),
				static_cast<float>(m_zSpinBox->value()),
				static_cast<float>(m_wSpinBox->value())
			});
	}

	void Vector4EditorWidget::onYChanged(double y)
	{
		emit dataChanged(
			{
				static_cast<float>(m_xSpinBox->value()),
				static_cast<float>(y),
				static_cast<float>(m_zSpinBox->value()),
				static_cast<float>(m_wSpinBox->value())
			});
	}

	void Vector4EditorWidget::onZChanged(double z)
	{
		emit dataChanged(
			{
				static_cast<float>(m_xSpinBox->value()),
				static_cast<float>(m_ySpinBox->value()),
				static_cast<float>(z),
				static_cast<float>(m_wSpinBox->value())
			});
	}

	void Vector4EditorWidget::onWChanged(double w)
	{
		emit dataChanged(
			{
				static_cast<float>(m_xSpinBox->value()),
				static_cast<float>(m_ySpinBox->value()),
				static_cast<float>(m_zSpinBox->value()),
				static_cast<float>(w)
			});
	}

	void Vector4EditorWidget::setData(const glm::vec4& data)
	{
		Vector3EditorWidget::setData(data.xyz);
		m_wSpinBox->setValue(data.w);
	}
}