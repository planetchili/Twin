#include "Shia.h"
#include "World.h"
#include "BvShiaSlowRoll.h"
#include "BvShiaChill.h"
#include "BvShiaDecide.h"

Shia::SlowRoll::SlowRoll( Shia& shia,const Vec2& target )
	:
	target( target )
{
	shia.isDoingBoundaryAdjustment = false;
}

Shia::SlowRoll::SlowRoll( Shia& shia,const Vec2& target,const Vec2& vel )
	:
	target( target )
{
	shia.isDoingBoundaryAdjustment = false;
	shia.vel = vel;
}

Shia::Behavior* Shia::SlowRoll::Update( Shia& shia,World& world,float dt )
{
	if( (target - shia.GetPos()).GetLengthSq() < 6.9f )
	{
		// now we are ready to live our lives in the bounded world
		shia.isDoingBoundaryAdjustment = true;
		// enter the charge loop by first doing a Chillout
		return new Decide( shia.rng );
	}

	shia.SetDirection( (target - shia.GetPos()).GetNormalized() );

	return nullptr;
}
