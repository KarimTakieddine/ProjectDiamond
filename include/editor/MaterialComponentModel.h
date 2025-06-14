#pragma once

#include "EngineMetaTypeRegistry.h"
#include "RenderComponentModel.h"

namespace diamond_engine { class MaterialComponentConfig; }

namespace project_diamond
{
	class MaterialComponentModel : public RenderComponentModel
	{
		Q_OBJECT

	public:
		MaterialComponentModel(QObject* parent = nullptr);

		void setData(diamond_engine::RenderComponentConfig* data) final override;
		const diamond_engine::RenderComponentConfig* getData() const final override;
		bool unparse(pugi::xml_node& node) const final override;

		const glm::vec3& getColor() const;
		const glm::vec2& getTextureOffset() const;
		QString getTextureName() const;

	signals:
		void colorChanged(const glm::vec3&);
		void textureOffsetChanged(const glm::vec2&);
		void textureNameChanged(const QString&);

	public slots:
		void setColor(const glm::vec3& color);
		void setTextureOffset(const glm::vec2& textureOffset);
		void setTextureName(const QString& textureName);

	private:
		diamond_engine::MaterialComponentConfig* m_data{ nullptr };
	};
}
