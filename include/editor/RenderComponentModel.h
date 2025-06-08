#pragma once

#include <QOBject>

namespace diamond_engine { class RenderComponentConfig; }

namespace project_diamond
{
	class RenderComponentModel : public QObject
	{
	public:
		RenderComponentModel(QObject* parent = nullptr) : QObject(parent) { }

		virtual void setData(diamond_engine::RenderComponentConfig* data)		= 0;
		virtual const diamond_engine::RenderComponentConfig* getData() const	= 0;
	};
}
