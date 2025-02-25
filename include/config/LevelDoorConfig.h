#pragma once

#include <string>

#include "config/BehaviourComponentConfig.h"

namespace project_diamond
{
	class LevelDoorConfig : public diamond_engine::BehaviourComponentConfig
	{
	public:
		~LevelDoorConfig() override = default;

		const char* getName() const final override;

		const std::string& getNextLevel() const;
		void setNextLevel(const std::string& name);

	private:
		std::string m_nextLevel{ "Unknown" };
	};
}
