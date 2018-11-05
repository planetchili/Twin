#include "BvPooOrbit.h"
#include "World.h"

Poo::Behavior* Poo::Orbit::Update( Poo& poo,World& world,float dt )
{
	s_time += dt;
	// calculate target position
	const auto tp = 
		Vec2{ radius,0.0f }
		// TODO: probs should clamp this to 2pi
		.Rotate( start_angle + orbit_ang_speed * s_time )
		+ world.GetShiaConst().GetPos();
	const auto delta = tp - poo.GetPos();
	const auto delta_len = delta.GetLength();
	// if we are close to target, adjust speed so that we don't overshoot it at update
	if( delta_len <= speed * dt )
	{
		poo.speed = delta_len / dt;
	}
	else
	{
		poo.speed = speed;
	}
	poo.SetDirection( delta / delta_len );
	return nullptr;
}

void Poo::Orbit::Activate( Poo& poo,const World& world )
{
	poo.bouncing = false;
}