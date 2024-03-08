#pragma once

#include <diamond_engine/behaviour/Behaviour.h>

class CharacterController2DBehaviour : public diamond_engine::Behaviour
{
public:
	void Update(GLfloat deltaTime) override;
};