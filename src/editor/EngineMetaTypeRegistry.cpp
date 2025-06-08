#include "EngineMetaTypeRegistry.h"

namespace project_diamond
{
	void registerEngineMetaTypes()
	{
		static bool registered(false);

		if (!registered)
		{
			qRegisterMetaType<glm::vec2>();
			qRegisterMetaType<glm::vec3>();
			qRegisterMetaType<glm::vec4>();

			registered = true;
		}
	}
}