#include "Vector2EditorWidget.h"

namespace project_diamond
{
	Vector2EditorWidget::Vector2EditorWidget(QWidget* parent /* = nullptr */) :
		QGroupBox	(parent),
		m_layout	(new QHBoxLayout()),
		m_xSpinBox	(new QDoubleSpinBox()),
		m_ySpinBox	(new QDoubleSpinBox())
	{

	}

	void Vector2EditorWidget::setupUi()
	{
		m_layout->addWidget(m_xSpinBox);
		m_layout->addWidget(m_ySpinBox);

		setLayout(m_layout);

		setMinimumSize(128, 64);
		setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
	}

	void Vector2EditorWidget::connectUi()
	{
		connect(m_xSpinBox, &QDoubleSpinBox::valueChanged, this, &Vector2EditorWidget::onXChanged);
		connect(m_ySpinBox, &QDoubleSpinBox::valueChanged, this, &Vector2EditorWidget::onYChanged);
	}

	void Vector2EditorWidget::onXChanged(double x)
	{
		emit dataChanged(
			{
					static_cast<float>(x),
					static_cast<float>(m_ySpinBox->value())
				});
	}

	void Vector2EditorWidget::onYChanged(double y)
	{
		emit dataChanged(
			{
				static_cast<float>(m_xSpinBox->value()),
				static_cast<float>(y)
			});
	}

	void Vector2EditorWidget::setData(const glm::vec2& data)
	{
		m_xSpinBox->setValue(data.x);
		m_ySpinBox->setValue(data.y);
	}
}