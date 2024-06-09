#pragma once

#include "Character2DConfig.h"

namespace project_diamond
{
	class DuckCharacterConfig : public Character2DConfig
	{
	public:
		~DuckCharacterConfig() final override = default;
		const char* getName() const final override;
	};
}
