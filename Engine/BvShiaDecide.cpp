#include "BvShiaDecide.h"
#include "bvshiachill.h"
#include "bvshiawigout.h"
#include "bvshiaeaseinto.h"
#include "bvshiacharge.h"

Shia::Decide::Decide( std::mt19937& rng )
	:
	rng( rng )
{
	OutputDebugString( L"decide create\n" );
}

Shia::Behavior* Shia::Decide::Update( Shia& shia,World& world,float dt )
{
	OutputDebugString( L"decide update\n" );

	enum class Move
	{
		Charge,
		Poop,
		Chasedown,
		Count
	};
	std::uniform_int_distribution<int> d_move( 0,0 );

	switch( (Move)d_move( rng ) )
	{
	case Move::Charge:
		SetupCharge();
		break;
	case Move::Poop:
		break;
	case Move::Chasedown:
		break;
	default:
		assert( false );
		break;
	}
	
	return PassTorch();
}

void Shia::Decide::SetupCharge()
{
	SetSuccessorStates( {
		new Decide( rng ),
		new Chill( 0.5f ),
		new Charge( 1400.0f,20.0f,500.5f ),
		new Wigout( 0.9f,0.025f,0.8f ),
		// why doesn't this work with <int>?
		new EaseInto( waypoints[std::uniform_int_distribution<size_t>{ 0,3 }( rng )],400.0f )
	} );
}
