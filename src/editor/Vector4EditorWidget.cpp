#include "Vector4EditorWidget.h"

namespace project_diamond
{
	Vector4EditorWidget::Vector4EditorWidget(QWidget* parent /* = nullptr */) :
		QGroupBox(parent),
		m_layout(new QHBoxLayout()),
		m_xSpinBox(new QDoubleSpinBox()),
		m_ySpinBox(new QDoubleSpinBox()),
		m_zSpinBox(new QDoubleSpinBox()),
		m_wSpinBox(new QDoubleSpinBox())
	{

	}

	void Vector4EditorWidget::setupUi()
	{
		m_layout->addWidget(m_xSpinBox);
		m_layout->addWidget(m_ySpinBox);
		m_layout->addWidget(m_zSpinBox);
		m_layout->addWidget(m_wSpinBox);

		setLayout(m_layout);
	}

	void Vector4EditorWidget::connectUi()
	{
		connect(m_xSpinBox, &QDoubleSpinBox::valueChanged, this, &Vector4EditorWidget::onXChanged);
		connect(m_ySpinBox, &QDoubleSpinBox::valueChanged, this, &Vector4EditorWidget::onYChanged);
		connect(m_zSpinBox, &QDoubleSpinBox::valueChanged, this, &Vector4EditorWidget::onZChanged);
		connect(m_wSpinBox, &QDoubleSpinBox::valueChanged, this, &Vector4EditorWidget::onWChanged);
	}

	void Vector4EditorWidget::onXChanged(double x)
	{
		const auto xFloat = static_cast<float>(x);
		if (xFloat == m_data.x)
		{
			return;
		}

		m_data.x = xFloat;

		emit dataChanged(m_data);
	}

	void Vector4EditorWidget::onYChanged(double y)
	{
		const auto yFloat = static_cast<float>(y);
		if (yFloat == m_data.y)
		{
			return;
		}

		m_data.y = yFloat;

		emit dataChanged(m_data);
	}

	void Vector4EditorWidget::onZChanged(double z)
	{
		const auto zFloat = static_cast<float>(z);
		if (zFloat == m_data.z)
		{
			return;
		}

		m_data.z = zFloat;

		emit dataChanged(m_data);
	}

	void Vector4EditorWidget::onWChanged(double w)
	{
		const auto wFloat = static_cast<float>(w);
		if (wFloat == m_data.w)
		{
			return;
		}

		m_data.w = wFloat;

		emit dataChanged(m_data);
	}

	void Vector4EditorWidget::setData(const glm::vec4& data)
	{
		m_xSpinBox->setValue(data.x);
		m_ySpinBox->setValue(data.y);
		m_zSpinBox->setValue(data.z);
		m_wSpinBox->setValue(data.w);

		m_data = data;
	}
}