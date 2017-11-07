#pragma once
#include "Shia.h"

class Shia::SlowRoll : public Shia::Behavior
{
public:
	SlowRoll( Shia& shia,const Vec2& target )
		:
		target( target )
	{
		shia.isDoingBoundaryAdjustment = false;
	}
	Behavior* Update( Shia& shia,class World& world,float dt ) override
	{
		if( (target - shia.GetPos()).GetLengthSq() < 6.9f )
		{
			// now we are ready to live our lives in the bounded world
			shia.isDoingBoundaryAdjustment = true;
			// enter the charge loop by first doing a Chillout
			return new ChillState( 0.25f );
		}

		shia.SetDirection( (target - shia.GetPos()).GetNormalized() );

		return nullptr;
	}
private:
	Vec2 target;
};