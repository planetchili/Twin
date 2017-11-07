#pragma once
#include "Shia.h"

class Shia::EaseInto : public Shia::Behavior
{
public:
	EaseInto( const Vec2& target,float speed );
	Behavior* Update( Shia& shia,class World& world,float dt ) override;
	virtual void Activate( Shia& shia,const class World& world ) override;
private:
	Vec2 target;
	float spd;
	float startDistance;
	float k;
};