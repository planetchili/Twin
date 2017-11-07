#include "BvShiaPoopin.h"
#include "World.h"

Shia::Poopin::Poopin( float duration,float period,float magnitude,int count,float poopin_shake_factor )
	:
	duration( duration ),
	period( period ),
	shake_dist( 0.0f,magnitude ),
	poopin_shake_factor( poopin_shake_factor ),
	oe( { 30.0f,-35.0f },{ -30.0f,-35.0f } )
{
	// generate ascending sequence of poo times
	std::uniform_real_distribution<float> pd( 0.0f,duration );
	ptimes.reserve( count );
	for( int n = 0; n < count; n++ )
	{
		ptimes.push_back( pd( rng ) );
	}
	std::sort( ptimes.begin(),ptimes.end() );
	iNextPoop = ptimes.cbegin();
}

Shia::Behavior* Shia::Poopin::Update( Shia& shia,World& world,float dt )
{
	// update state duration timer
	s_time += dt;

	// deliver the scheduled poops
	int poop_count = 0;
	while( iNextPoop != ptimes.cend() && s_time >= *iNextPoop )
	{
		world.SpawnPoo( shia.pos + oe.GetOffset( shia.facingRight ),
			(Vec2( 400.0f,300.0f ) - shia.pos).GetNormalized()
			.GetRotated( angle_dist( rng ) )
			* speed_dist( rng )
		);
		poop_count++;
		iNextPoop++;
	}
	// play poop sound and shaker hard if at least one poop poopered
	if( poop_count > 0 )
	{
		shia.poop_sound->Play( 1.0f,0.5f );
		shia.pos = base + Vec2{ shake_dist( rng ),shake_dist( rng ) } *poopin_shake_factor;
	}

	// ending condition is if time has elapsed
	if( s_time >= duration )
	{
		if( HasSuccessors() )
		{
			return PassTorch();
		}
	}

	// update vibration timer if not poo shakin
	if( poop_count == 0 )
	{
		v_time += dt;
		if( v_time >= period )
		{
			shia.pos = base + Vec2{ shake_dist( rng ),shake_dist( rng ) };
			v_time = 0.0f;
		}
	}

	// maintain current state
	return nullptr;
}

void Shia::Poopin::Activate( Shia& shia,const World& world )
{
	base = shia.GetPos();
	shia.sprite.SetMode( SpriteMode::Pooping );
}
