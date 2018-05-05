#include "BvPooPursue.h"
#include "Poo.h"
#include "World.h"

Poo::Behavior* Poo::Pursue::Update( Poo& poo,World& world,float dt )
{
	s_time += dt;
	if( s_time >= duration && HasSuccessors() )
	{
		poo.SetSpeed( 0.0f );
		return PassTorch();
	}

	poo.speed = speed;
	// flag for avoidance state
	bool avoiding = false;
	// if close to any other poo, avoid it
	for( auto& other : world.GetPoosConst() )
	{
		// don't consider self
		if( &poo == &other )
		{
			continue;
		}
		// check if poo is within theshold
		const auto delta = poo.GetPos() - other.GetPos();
		const auto lensq = delta.GetLengthSq();
		if( lensq < avoid_sq )
		{
			// avoiding state set
			avoiding = true;
			// case for poos at same location
			if( lensq == 0.0f )
			{
				 poo.SetDirection( { -1.0f,1.0f } );
			}
			else
			{
				// normalize delta to get dir (reusing precalculated lensq)
				// if you would have just called Normalize() like a good boy...
				poo.SetDirection( delta / std::sqrt( lensq ) );
			}
			// no need to check other poos
			break;
		}
	}
	// check if in avoidance state, if so do not pursue
	if( !avoiding )
	{
		const auto delta = world.GetChiliConst().GetPos() - poo.GetPos();
		// we only wanna move if not already really close to target pos
		// (prevents vibrating around target point)
		if( delta.GetLengthSq() > settle_sq )
		{
			poo.SetDirection( delta.GetNormalized() );
		}
		else
		{
			poo.SetDirection( { 0.0f,0.0f } );
		}
	}
	return nullptr;
}
