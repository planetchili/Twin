#include "Shia.h"
#include "World.h"
#include "BvShiaWigout.h"

Shia::Wigout::Wigout( float duration,float period,float magnitude )
	:
	duration( duration ),
	period( period ),
	dist( 0.0f,magnitude )
{}

Shia::Behavior* Shia::Wigout::Update( Shia& shia,World& world,float dt )
{
	// update state duration timer
	s_time += dt;
	// ending condition is if time has elapsed
	if( s_time >= duration )
	{
		if( HasSuccessors() )
		{
			return PassTorch();
		}
	}

	// update vibration timer
	v_time += dt;
	if( v_time >= period )
	{
		shia.pos = base + Vec2{ dist( rng ),dist( rng ) };
		v_time = 0.0f;
	}

	// maintain current state
	return nullptr;
}

void Shia::Wigout::Activate( Shia& shia,const World& world )
{
	base = shia.GetPos();
	if( base.x > 400.0f )
	{
		shia.facingRight = true;
	}
	shia.sprite.SetMode( SpriteMode::Pooping );
	shia.sprite.Reset();
}
