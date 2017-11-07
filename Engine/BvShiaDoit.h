#pragma once
#include "Shia.h"

class Shia::Doit : public Shia::Behavior
{
public:
	Behavior* Update( Shia& shia,class World& world,float dt ) override;
	virtual void Activate( Shia& shia,const class World& world ) override;
};