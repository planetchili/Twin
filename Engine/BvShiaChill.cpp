#include "Shia.h"
#include "World.h"
#include "BvShiaChill.h"
#include "BvShiaWigout.h"
#include "BvShiaPoopin.h"
#include "BvShiaDoit.h"
#include "BvShiaEaseInto.h"
#include "BvShiaBeamer.h"

Shia::Chill::Chill( float duration )
	:
	duration( duration )
{}

Shia::Behavior* Shia::Chill::Update( Shia& shia,World& world,float dt )
{
	s_time += dt;
	if( s_time >= duration )
	{
		return PassTorch();
	}

	return nullptr;
}

void Shia::Chill::Activate( Shia& shia,const World& world )
{
	shia.sprite.SetMode( SpriteMode::Standing );
}
