#pragma once

#include <QHash>
#include <QObject>
#include <QPixmap>
#include <QString>

namespace project_diamond
{
	class TextureModel : public QObject
	{
		Q_OBJECT

	public:
		TextureModel(QObject* parent = nullptr);

		bool load(const QString& directory);

	signals:
		void imageChanged(const QPixmap&) const;

	public slots:
		void findImage(const QString& name) const;

	private:
		QHash<QString, QPixmap> m_images;
	};
}
