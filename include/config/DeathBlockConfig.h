#pragma once

#include <glm/vec3.hpp>

#include <config/BehaviourComponentConfig.h>

namespace project_diamond
{
	class DeathBlockConfig : public diamond_engine::BehaviourComponentConfig
	{
	public:
		~DeathBlockConfig() override = default;

		const char* getName() const final override;

		const glm::vec3& getResetPosition() const;
		void setResetPosition(const glm::vec3& resetPosition);

	private:
		glm::vec3 m_resetPosition{ 0.0f, 0.0f, 0.0f };
	};
}
