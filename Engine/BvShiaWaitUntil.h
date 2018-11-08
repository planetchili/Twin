#pragma once
#include "Shia.h"

class Shia::WaitUntil : public Shia::Behavior
{
public:
	WaitUntil( float time );
	Behavior* Update( Shia& shia,class World& world,float dt ) override;
	void Activate( Shia& shia,const class World& world ) override;
private:
	float time;
};