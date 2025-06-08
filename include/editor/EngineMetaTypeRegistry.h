#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include <QMetaType>

Q_DECLARE_METATYPE(glm::vec2);
Q_DECLARE_METATYPE(glm::vec3);
Q_DECLARE_METATYPE(glm::vec4);

namespace project_diamond
{
	void registerEngineMetaTypes();
}
