#include <pugixml.hpp>

#include "Character2DConfigParser.h"

namespace project_diamond
{
	std::unique_ptr<diamond_engine::BehaviourComponentConfig> Character2DConfigParser::parse(const pugi::xml_node& behaviourNode, diamond_engine::EngineStatus* outStatus /* = nullptr */)
	{
		return std::make_unique<Character2DConfig>();
	}
}