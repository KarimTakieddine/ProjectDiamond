#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include <GameInstanceConfig.h>

#include <QMetaType>

Q_DECLARE_METATYPE(glm::vec2);
Q_DECLARE_METATYPE(glm::vec3);
Q_DECLARE_METATYPE(glm::vec4);

Q_DECLARE_METATYPE(diamond_engine::GameInstanceType);

namespace project_diamond
{
	void registerEngineMetaTypes();
}
