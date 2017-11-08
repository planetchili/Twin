#pragma once
#include "Shia.h"

class Shia::Beamer : public Shia::Behavior
{
public:
	Behavior* Update( Shia& shia,class World& world,float dt ) override;
	virtual void Activate( Shia& shia,const class World& world ) override;
public:
	float state_time = 0.0f;
};