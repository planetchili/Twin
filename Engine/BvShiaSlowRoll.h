#pragma once
#include "Shia.h"

class Shia::SlowRoll : public Shia::Behavior
{
public:
	SlowRoll( Shia& shia,const Vec2& target );
	SlowRoll( Shia& shia,const Vec2& target,float vel );
	Behavior* Update( Shia& shia,class World& world,float dt ) override;
	void Activate( Shia& shia,const class World& world ) override;
private:
	Vec2 target;
	float speed = 1.0f;
};