#include "BvShiaDecide.h"
#include "bvshiachill.h"
#include "bvshiawigout.h"
#include "bvshiaeaseinto.h"
#include "bvshiacharge.h"
#include "BvShiaPoopin.h"
#include "BvShiaChasedown.h"
#include "BvShiaBeamer.h"
#include "BvShiaFling.h"
#include "BvShiaAwaken.h"
#include "BvShiaOrbit.h"
#include "BvShiaSlowRoll.h"
#include "BvShiaWaitUntil.h"

Shia::Decide::Decide( std::mt19937& rng )
	:
	rng( rng )
{}

Shia::Behavior* Shia::Decide::Update( Shia& shia,World& world,float dt )
{
	// check to see if it is TIME
	if( shia.timer.Peek() >= 68.0f )
	{
		SetSuccessorStates( {
			new Beamer,
			new WaitUntil( 72.4f ),
			new EaseInto( { 400.0f,300.0f },400.0f )
		} );
		return PassTorch();
	}

	enum class Move
	{
		Charge,
		Poop,
		Fling,
		Orbit,
		Count
	};
	std::discrete_distribution<> d_move( { 40,40,60,30 } );

	switch( (Move)d_move( rng ) )
	{
	case Move::Charge:
		PushSuccessorState( new Decide( rng ) );
		PushSuccessorStates( MakeChargeSequence( rng ) );
		break;
	case Move::Poop:
		PushSuccessorState( new Decide( rng ) );
		PushSuccessorStates( MakePoopinSequence( rng ) );
		break;
	case Move::Fling:
		SetSuccessorStates( {
			new Decide( rng ),
			new Fling( rng ),
			new Fling( rng ),
			new Fling( rng ),
		} );
		break;
	case Move::Orbit:
		SetSuccessorStates( {
			new Decide( rng ),
			new Chasedown( 4.0f ),
			new Orbit( rng ),
		} );
		break;
	default:
		assert( false );
		break;
	}
	
	return PassTorch();
}

std::vector<Shia::Behavior*> Shia::Decide::MakePoopinSequence( std::mt19937& rng )
{
	return  {
		new Chill( 0.5f ),
		new Poopin( 1.25f,0.035f,1.0f,7,9.0f ),
		new Wigout( 0.8f,0.025f,0.8f ),
		new EaseInto( waypoints[std::uniform_int_distribution<size_t>{ 0,3 }(rng)],400.0f )
	};
}

std::vector<Shia::Behavior*> Shia::Decide::MakeChargeSequence( std::mt19937& rng )
{
	return  {
		new Chill( 0.5f ),
		new Charge( 1400.0f,20.0f,500.5f ),
		new Wigout( 0.9f,0.025f,0.8f ),
		new EaseInto( waypoints[std::uniform_int_distribution<size_t>{ 0,3 }(rng)],400.0f )
	};
}
