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
	shia.vel = (world.GetChiliConst().GetPos() - shia.pos).GetNormalized() * chase_speed;

	// maintain current state
	return nullptr;
}

void Shia::Chasedown::Activate( Shia& shia,const World& world )
{
}
