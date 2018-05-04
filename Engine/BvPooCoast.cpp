#include "BvPooCoast.h"

Poo::Behavior* Poo::Coast::Update( Poo& poo,World& world,float dt )
{
	if( poo.speed == 0.0f )
	{
		if( HasSuccessors() )
		{
			return PassTorch();
		}
	}

	poo.SetSpeed( std::max( poo.speed - decel_per_sec * dt,0.0f ) );
	return nullptr;
}
