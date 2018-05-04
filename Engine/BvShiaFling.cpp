#include "BvShiaFling.h"
#include "World.h"
#include "Shia.h"
#include "Poo.h"
#include <algorithm>
#include <iterator>


Shia::Fling::Fling( std::mt19937& rng )
	:
	rng( rng )
{}

Shia::Behavior* Shia::Fling::Update( Shia& shia,World& world,float dt )
{
	s_time += dt;
	if( s_time >= duration || !activated )
	{
		if( HasSuccessors() )
		{
			return PassTorch();
		}
	}

	shia.SetSpeed( 0.0f );

	return nullptr;
}

void Shia::Fling::Activate( Shia& shia,const World& world )
{
	auto& poos = world.GetPoosConst();
	if( poos.size() > 0 )
	{
		// generate list of refs to candidate poos
		std::vector<std::reference_wrapper<const Poo>> candidates;
		std::copy_if( poos.begin(),poos.end(),std::back_inserter( candidates ),
			[]( const Poo& poo ) { return poo.GetVel().GetLengthSq() == 0.0f;}
		);
		if( candidates.size() > 0 )
		{
			// select a poo from the candidates and fling it
			std::uniform_int_distribution<size_t> dist( 0,candidates.size() - 1 );
			// hack:chili (should World be const here?)
			const_cast<Poo&>(candidates[dist( rng )].get()).Fling();
			activated = true;
		}
	}
}

