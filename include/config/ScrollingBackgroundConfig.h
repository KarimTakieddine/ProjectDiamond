#pragma once

#include <glm/vec2.hpp>

#include <BehaviourComponentConfig.h>

namespace project_diamond
{
	class ScrollingBackgroundConfig : public diamond_engine::BehaviourComponentConfig
	{
	public:
		~ScrollingBackgroundConfig() final override = default;
		const char* getName() const final override;

		void setSpeed(const glm::vec2& speed);
		const glm::vec2& getSpeed() const;

	private:
		glm::vec2 m_speed{ 0.0f, 0.0f };
	};
}
