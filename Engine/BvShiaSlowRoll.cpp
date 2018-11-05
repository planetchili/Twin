#include "Shia.h"
#include "World.h"
#include "BvShiaSlowRoll.h"
#include "BvShiaChill.h"
#include "BvShiaDecide.h"

Shia::SlowRoll::SlowRoll( Shia& shia,const Vec2& target )
	:
	target( target )
{}

Shia::SlowRoll::SlowRoll( Shia& shia,const Vec2& target,float speed )
	:
	target( target ),
	speed( speed )
{}

Shia::Behavior* Shia::SlowRoll::Update( Shia& shia,World& world,float dt )
{
	if( (target - shia.GetPos()).GetLengthSq() < 6.9f )
	{
		// now we are ready to live our lives in the bounded world
		// special bullshit for the entrance sequence
		shia.isDoingBoundaryAdjustment = true;

		return PassTorch();
	}

	shia.SetDirection( (target - shia.GetPos()).GetNormalized() );

	return nullptr;
}

void Shia::SlowRoll::Activate( Shia& shia,const World& world )
{
	shia.SetSpeed( speed );
}
