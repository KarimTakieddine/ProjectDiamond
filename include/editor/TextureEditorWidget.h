#pragma once

#include <QLabel>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QPixmap>
#include <QVBoxLayout>
#include <QGroupBox>

namespace project_diamond
{
	class TextureEditorWidget : public QGroupBox
	{
		Q_OBJECT

	public:
		TextureEditorWidget(QWidget* parent = nullptr);

		void setupUi();
		void connectUi();

	signals:
		void nameChanged(const QString& name);

	public slots:
		void setName(const QString& name);
		void setImage(const QPixmap& image);

	private slots:
		void onNameEditingFinished();

	private:
		QVBoxLayout* m_layout			{ nullptr };
		QHBoxLayout* m_textureNameLayout{ nullptr };
		QLabel* m_textureNameLabel		{ nullptr };
		QLineEdit* m_textureNameLineEdit{ nullptr };
		QLabel* m_textureImageLabel		{ nullptr };
	};
}
