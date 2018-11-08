#include "BvShiaChasedown.h"
#include "World.h"

Shia::Chasedown::Chasedown( float duration )
	:
	duration( duration )
{}

Shia::Behavior* Shia::Chasedown::Update( Shia& shia,World& world,float dt )
{
	// ending condition is if speed goes slow enough
	time += dt;
	if( time > duration )
	{
		if( HasSuccessors() )
		{
			return PassTorch();
		}
		assert( false );
	}

	// constant deceleration independent of velocity
	const auto delta = world.GetChiliConst().GetPos() - shia.pos;
	shia.speed = std::min( max_speed,chase_speed + k * (delta.GetLengthSq() - chase_dist_sq) );
	shia.SetDirection( delta.GetNormalized() );

	// maintain current state
	return nullptr;
}
