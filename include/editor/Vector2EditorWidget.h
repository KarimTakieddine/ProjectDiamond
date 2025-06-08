#pragma once

#define GLM_FORCE_SWIZZLE

#include <QGroupBox>
#include <QDoubleSpinBox>
#include <QHBoxLayout>

#include "EngineMetaTypeRegistry.h"

namespace project_diamond
{
	class Vector2EditorWidget : public QGroupBox
	{
		Q_OBJECT

	public:
		Vector2EditorWidget(QWidget* parent = nullptr);

		virtual void setupUi();
		virtual void connectUi();

	public slots:
		void setData(const glm::vec2& data);

	signals:
		void dataChanged(const glm::vec2& data);

	protected slots:
		virtual void onXChanged(double x);
		virtual void onYChanged(double y);

	protected:
		QHBoxLayout* m_layout		{ nullptr };
		QDoubleSpinBox* m_xSpinBox	{ nullptr };
		QDoubleSpinBox* m_ySpinBox	{ nullptr };
	};
}
