#pragma once

#include <functional>

#include <QHash>
#include <QPixmap>
#include <QWidget>
#include <QVBoxLayout>

QT_BEGIN_NAMESPACE
namespace Ui { class ComponentEditorWidget; };
QT_END_NAMESPACE

namespace project_diamond
{
	class RenderComponentModel;
	class TextureModel;
	class ComponentEditorWidget : public QWidget
	{
		Q_OBJECT

	public:
		ComponentEditorWidget(QWidget* parent = nullptr);

		void setupUi();
		void connectUi();
		void clear();

		bool configureComponent(RenderComponentModel* component, const QString& name);
		void setTextureModel(TextureModel* model);

	signals:
		void imageChanged(const QPixmap&);

	private:
		void insertWidget(QWidget* widget, int index);
		
		bool configureTransformComponent(RenderComponentModel* component);
		bool configureMaterialComponent(RenderComponentModel* component);

		QHash<QString, std::function<bool(RenderComponentModel*)>> m_configureFunctions;
		QVector<QWidget*> m_childWidgets;
		QVBoxLayout* m_layout		{ nullptr };
		TextureModel* m_textureModel{ nullptr };
	};
}
