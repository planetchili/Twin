#pragma once
#include "Shia.h"

class Shia::Charge : public Shia::Behavior
{
public:
	Charge( float start_speed,float end_speed,float decel_k );
	Behavior* Update( Shia& shia,class World& world,float dt ) override;
	virtual void Activate( Shia& shia,const class World& world ) override;
private:
	float start_speed;
	// used for transition condition
	float end_speed_sq;
	// used to calculate drag/friction
	float decel_k;
};