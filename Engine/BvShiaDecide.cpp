#include "BvShiaDecide.h"
#include "bvshiachill.h"
#include "bvshiawigout.h"
#include "bvshiaeaseinto.h"
#include "bvshiacharge.h"
#include "BvShiaPoopin.h"
#include "BvShiaChasedown.h"
#include "BvShiaBeamer.h"

Shia::Decide::Decide( std::mt19937& rng )
	:
	rng( rng )
{}

Shia::Behavior* Shia::Decide::Update( Shia& shia,World& world,float dt )
{
	enum class Move
	{
		Charge,
		Poop,
		Chasedown,
		Beam,
		Count
	};
	std::uniform_int_distribution<int> d_move( 0,(int)Move::Count - 1 );

	switch( Move::Poop )
	{
	case Move::Charge:
		SetSuccessorStates( {
			new Decide( rng ),
			new Chill( 0.5f ),
			new Charge( 1400.0f,20.0f,500.5f ),
			new Wigout( 0.9f,0.025f,0.8f ),
			// why doesn't this work with <int>?
			new EaseInto( waypoints[std::uniform_int_distribution<size_t>{ 0,3 }( rng )],400.0f )
		} );
		break;
	case Move::Poop:
		SetSuccessorStates( {
			new Decide( rng ),
			new Chill( 0.5f ),
			new Poopin( 1.25f,0.035f,1.0f,7,9.0f ),
			new Wigout( 0.8f,0.025f,0.8f ),
			// why doesn't this work with <int>?
			new EaseInto( waypoints[std::uniform_int_distribution<size_t>{ 0,3 }( rng )],400.0f )
		} );
		break;
	case Move::Chasedown:
		SetSuccessorStates( {
			new Decide( rng ),
			new Chill( 0.5f ),
			new Chasedown( 2.5f )
		} );
		break;
	case Move::Beam:
		SetSuccessorStates( {
			new Beamer,
			new Chill( 0.3f ),
			new EaseInto( { 400.0f,300.0f },400.0f )
		} );
		break;
	default:
		assert( false );
		break;
	}
	
	return PassTorch();
}
