#pragma once

#include "EngineMetaTypeRegistry.h"
#include "RenderComponentModel.h"

namespace diamond_engine { class TransformComponentConfig; }

namespace project_diamond
{
	class TransformComponentModel : public RenderComponentModel
	{
		Q_OBJECT

	public:
		TransformComponentModel(QObject* parent = nullptr);

		void setData(diamond_engine::RenderComponentConfig* data) final override;
		const diamond_engine::RenderComponentConfig* getData() const final override;

		const glm::vec3& getPosition() const;
		const glm::vec3& getLocalScale() const;

	signals:
		void positionChanged(const glm::vec3&);
		void localScaleChanged(const glm::vec3&);

	public slots:
		void setPosition(const glm::vec3& position);
		void setLocalScale(const glm::vec3& localScale);

	private:
		diamond_engine::TransformComponentConfig* m_data{ nullptr };
	};
}
