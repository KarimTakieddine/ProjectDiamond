#pragma once

#include "Vector3EditorWidget.h"

namespace project_diamond
{
	class Vector4EditorWidget : public Vector3EditorWidget
	{
		Q_OBJECT

	public:
		Vector4EditorWidget(QWidget* parent = nullptr);

		void setupUi() final override;
		void connectUi() final override;

	public slots:
		void setData(const glm::vec4& data);

	signals:
		void dataChanged(const glm::vec4& data);

	protected slots:
		void onXChanged(double x) final override;
		void onYChanged(double y) final override;
		void onZChanged(double z) final override;
		void onWChanged(double w);

	protected:
		QDoubleSpinBox* m_wSpinBox{ nullptr };
	};
}
