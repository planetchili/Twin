#pragma once
#include "Shia.h"

// faster when further
class Shia::Chasedown : public Shia::Behavior
{
public:
	Chasedown( float duration );
	Behavior* Update( Shia& shia,class World& world,float dt ) override;
	void Activate( Shia& shia,const class World& world ) override;
private:
	float chase_speed = 75.0f;
	float duration;
	float time = 0.0f;
};
