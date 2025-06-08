#pragma once

#include "Vector2EditorWidget.h"

namespace project_diamond
{
	class Vector3EditorWidget : public Vector2EditorWidget
	{
		Q_OBJECT

	public:
		Vector3EditorWidget(QWidget* parent = nullptr);

		virtual void setupUi() override;
		virtual void connectUi() override;

	public slots:
		void setData(const glm::vec3& data);

	signals:
		void dataChanged(const glm::vec3& data);

	protected slots:
		virtual void onXChanged(double x) override;
		virtual void onYChanged(double y) override;
		virtual void onZChanged(double z);

	protected:
		QDoubleSpinBox* m_zSpinBox{ nullptr };
	};
}
