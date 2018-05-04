#include "BvPooCoast.h"

Poo::Behavior* Poo::Coast::Update( Poo& poo,World& world,float dt )
{
	poo.SetSpeed( std::max( poo.speed - decel_per_sec * dt,0.0f ) );
	return nullptr;
}
