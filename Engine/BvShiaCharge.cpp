#include "World.h"
#include "BvShiaCharge.h"

Shia::Charge::Charge( float start_speed,float end_speed,float decel_k )
	:
	start_speed( start_speed ),
	end_speed_sq( sq( end_speed ) ),
	decel_k( decel_k )
{}

Shia::Behavior* Shia::Charge::Update( Shia& shia,World& world,float dt )
{
	// ending condition is if speed goes slow enough
	if( shia.vel.GetLengthSq() < end_speed_sq )
	{
		if( HasSuccessors() )
		{
			return PassTorch();
		}
	}

	// constant deceleration independent of velocity
	// TODO: refactor to use setspeed()
	shia.vel -= shia.vel.GetNormalized() * decel_k * dt;

	// maintain current state
	return nullptr;
}

void Shia::Charge::Activate( Shia& shia,const World& world )
{
	shia.speed = start_speed;
	// target chili
	shia.SetDirection( (world.GetChiliConst().GetPos() - shia.GetPos()).GetNormalized() );
	shia.sprite.SetMode( SpriteMode::Standing );
}
