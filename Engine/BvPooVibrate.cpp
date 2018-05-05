#include "Poo.h"
#include "World.h"
#include "BvPooVibrate.h"
#include "BvPooCoast.h"

Poo::Behavior* Poo::Vibrate::Update( Poo& poo,World& world,float dt )
{
	// update state duration timer
	s_time += dt;
	// ending condition is if time has elapsed
	if( s_time >= duration )
	{
		return PassTorch();
	}

	// set velocity for vibration effect
	poo.speed = amplitude * sin( (2.0f * PI / period) * s_time );
	poo.SetDirection( { 1.0f,0.0f } );

	// maintain current state
	return nullptr;
}