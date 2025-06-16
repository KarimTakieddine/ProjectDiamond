#include "TextureEditorWidget.h"

namespace project_diamond
{
	TextureEditorWidget::TextureEditorWidget(QWidget* parent /* = nullptr */) :
		QGroupBox				(),
		m_layout				(new QVBoxLayout()),
		m_textureNameLayout		(new QHBoxLayout()),
		m_textureNameLabel		(new QLabel()),
		m_textureNameLineEdit	(new QLineEdit()),
		m_textureImageLabel		(new QLabel())
	{

	}

	void TextureEditorWidget::setupUi()
	{
		m_textureNameLabel->setText(QStringLiteral("Name"));
		m_textureNameLabel->setMinimumSize(64, 28);

		m_textureNameLayout->addWidget(m_textureNameLabel);
		m_textureNameLayout->addWidget(m_textureNameLineEdit);
		m_textureNameLayout->setStretch(m_textureNameLayout->count() - 1, 1);
		m_layout->addLayout(m_textureNameLayout);

		m_textureImageLabel->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
		m_layout->addWidget(m_textureImageLabel);

		m_layout->setStretch(m_layout->count() - 1, 1);
		setLayout(m_layout);

		setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
		setMinimumSize(128, 128);
	}

	void TextureEditorWidget::connectUi()
	{
		connect(m_textureNameLineEdit, &QLineEdit::editingFinished, this, &TextureEditorWidget::onNameEditingFinished);
	}

	void TextureEditorWidget::setName(const QString& name)
	{
		if (name == m_textureNameLineEdit->text())
		{
			return;
		}

		m_textureNameLineEdit->setText(name);
	}

	void TextureEditorWidget::setImage(const QPixmap& image)
	{
		m_textureImageLabel->setPixmap(image);

		if (image.isNull())
		{
			return;
		}
		
		m_textureImageLabel->setFixedWidth(image.width());
		m_textureImageLabel->setFixedHeight(image.height());
	}

	void TextureEditorWidget::onNameEditingFinished()
	{
		emit nameChanged(m_textureNameLineEdit->text());
	}
}