#include "BvShiaWaitUntil.h"

Shia::WaitUntil::WaitUntil( float time )
	:
	time( time )
{}

Shia::Behavior* Shia::WaitUntil::Update( Shia& shia,World& world,float dt )
{
	// check to see if it is TIME
	if( shia.timer.Peek() >= time )
	{
		return PassTorch();
	}

	return nullptr;
}

void Shia::WaitUntil::Activate( Shia& shia,const class World& world )
{
	shia.SetSpeed( 0.0f );
}
