#include <filesystem>

#include <TextureMetadataParser.h>

#include "TextureModel.h"

namespace project_diamond
{
	TextureModel::TextureModel(QObject* parent /* = nullptr */) { }

	bool TextureModel::load(const QString& directory)
	{
		m_images.clear();

		const std::filesystem::path rootDirectoryPath(directory.toStdString());
		if (!std::filesystem::is_directory(rootDirectoryPath))
		{
			// TODO
			return false;
		}

		const std::filesystem::path textureMetadataPath = rootDirectoryPath / "textureMetadata.xml";
		if (!std::filesystem::is_regular_file(textureMetadataPath))
		{
			// TODO
			return false;
		}

		const auto textureMetaData = diamond_engine::TextureMetadataParser::ParseCollection(textureMetadataPath.string());

		for (const auto& entry : textureMetaData.GetTextures())
		{
			std::filesystem::path texturePath = rootDirectoryPath / entry.GetPath();
			if (!std::filesystem::is_regular_file(texturePath))
			{
				// TODO
				continue;
			}

			texturePath.make_preferred();

			QPixmap texture;
			texture.load(QString::fromStdString(texturePath.string()));
			if (texture.isNull())
			{
				// TODO
				continue;
			}

			m_images.insert(QString::fromStdString(entry.GetName()), texture);
		}

		return true;
	}

	void TextureModel::findImage(const QString& name) const
	{
		auto it = m_images.constFind(name);
		if (it == m_images.constEnd())
		{
			// TODO
			emit imageChanged(QPixmap());
			return;
		}

		emit imageChanged(it.value());
	}
}