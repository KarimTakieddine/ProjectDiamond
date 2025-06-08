#pragma once

#include <QGroupBox>
#include <QDoubleSpinBox>
#include <QHBoxLayout>

#include "EngineMetaTypeRegistry.h"

namespace project_diamond
{
	class Vector4EditorWidget : public QGroupBox
	{
		Q_OBJECT

	public:
		Vector4EditorWidget(QWidget* parent = nullptr);

		void setupUi();
		void connectUi();

	public slots:
		void setData(const glm::vec4& data);

	signals:
		void dataChanged(const glm::vec4& data);

	private slots:
		void onXChanged(double x);
		void onYChanged(double y);
		void onZChanged(double z);
		void onWChanged(double w);

	private:
		QHBoxLayout* m_layout		{ nullptr };
		QDoubleSpinBox* m_xSpinBox	{ nullptr };
		QDoubleSpinBox* m_ySpinBox	{ nullptr };
		QDoubleSpinBox* m_zSpinBox	{ nullptr };
		QDoubleSpinBox* m_wSpinBox	{ nullptr };
		glm::vec4 m_data			{ 0.0f, 0.0f, 0.0f, 0.0f };
	};
}
