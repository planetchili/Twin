#include "BvShiaDoit.h"
#include "World.h"

Shia::Behavior* Shia::Doit::Update( Shia& shia,World& world,float dt )
{
	// immediately transition
	if( HasSuccessors() )
	{
		return PassTorch();
	}

	// maintain current state
	return nullptr;
}

void Shia::Doit::Activate( Shia& shia,const World& world )
{
	shia.doit_sound1->Play();
}
