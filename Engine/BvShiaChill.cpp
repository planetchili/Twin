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
		// stuff for choosing where to charge from
		static std::mt19937 rng( std::random_device{}() );
		std::uniform_int_distribution<int> dist( 0,3 );
		static const Vec2 waypoints[] = {
			{ 150.0f,150.0f },
			{ 650.0f,150.0f },
			{ 150.0f,450.0f },
			{ 650.0f,450.0f }
		};
		// setup the successor sequence which will follow this state
		// (states will execute bottom to top)
		//SetSuccessorStates( {
		//	new Chill( 0.5f ),
		//	new Poopin( 1.25f,0.035f,1.0f,7,9.0f ),
		//	new Wigout( 0.9f,0.025f,0.8f ),
		//	new Doit,
		//	new EaseInto( waypoints[dist( rng )],400.0f )
		//} );
		return new Beamer;
	}

	shia.SetDirection( { 0.0f,0.0f } );
	return nullptr;
}

void Shia::Chill::Activate( Shia& shia,const World& world )
{
	shia.sprite.SetMode( SpriteMode::Standing );
}
