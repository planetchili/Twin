#pragma once
#include "Shia.h"

class Shia::SlowRoll : public Shia::Behavior
{
public:
	SlowRoll( Shia& shia,const Vec2& target );
	SlowRoll( Shia& shia,const Vec2& target,const Vec2& vel );
	Behavior* Update( Shia& shia,class World& world,float dt ) override;
private:
	Vec2 target;
};