#include "Shia.h"
#include "SpriteEffect.h"
#include "World.h"

Shia::Shia( const Vec2& pos )
	:
	Entity( pos,75.0f,90.0f,60.0f )
{
	// [0] shia standing
	spritePtrs.push_back( new CompositeSpriteElement( {
		new SurfaceSpriteElement( Codex<Surface>::Retrieve( L"Images\\pm_roomba_right.png" ),
			{ -47.0f,-33.0f },{ -47.0f,-33.0f }
		),
		new SurfaceSpriteElement( Codex<Surface>::Retrieve( L"Images\\pm_shia_right.png" ),
			{ -26.0f,-161.0f },{ -26.0f,-161.0f }
		)
	} )	);
	// [1] shia poopin
	spritePtrs.push_back( new CompositeSpriteElement( {
		new SurfaceSpriteElement( Codex<Surface>::Retrieve( L"Images\\pm_roomba_right.png" ),
			{ -47.0f,-33.0f },{ -47.0f,-33.0f }
		),
		new AnimationSpriteElement(
			0,0,99,154,6,
			Codex<Surface>::Retrieve( L"Images\\pm_shia_poopin.png" ),
			{ 0.11f,0.11f,0.11f,0.11f,0.18f,std::numeric_limits<float>::max() },
			{ -66.0f,-160.0f },
			{ -35.0f,-160.0f }
		)
	} ) );
}

Shia::~Shia()
{
	for( auto p : spritePtrs )
	{
		delete p;
	}
}

void Shia::ProcessLogic( const World& world )
{
	pBrainState->ProcessLogic( *this,world );
}

void Shia::Update( World& world,float dt )
{
	pos += vel * dt;

	if( isDoingBoundaryAdjustment )
	{
		world.GetBoundsConst().Adjust( *this );
	}

	if( effectActive )
	{
		effectTime += dt;
		if( effectTime >= effectDuration )
		{
			effectActive = false;
		}
	}

	// handle brain state transition / update
	// delete old state if new one is returned
	// call activate on new state for 2nd part of init
	// and call update on the new one (repeat)
	while( auto pNewState = pBrainState->Update( *this,world,dt ) )
	{
		delete pBrainState;
		pBrainState = pNewState;
		pBrainState->Activate( *this,world );
	}

	GetCurrentSprite().Update( dt );
}

void Shia::ApplyDamage( float damage )
{
	effectActive = true;
	effectTime = 0.0f;
}

void Shia::Draw( Graphics& gfx ) const
{
	if( effectActive ) // draw damage flash
	{
		GetCurrentSprite().SetEffectColor( Colors::White );
		GetCurrentSprite().Draw( pos,gfx.GetScreenRect(),gfx,SpriteElement::Effect::ColorSub,facingLeft );
	}
	else // draw normal
	{
		GetCurrentSprite().Draw( pos,gfx.GetScreenRect(),gfx,SpriteElement::Effect::None,facingLeft );
	}
}

Shia::SlowRollState::SlowRollState( Shia& shia,const Vec2& target )
	:
	target( target )
{
	shia.isDoingBoundaryAdjustment = false;
}

void Shia::SlowRollState::ProcessLogic( Shia& shia,const World& world ) const
{
	shia.SetDirection( (target - shia.GetPos()).GetNormalized() );
}

Shia::BrainState* Shia::SlowRollState::Update( Shia& shia,World& world,float dt )
{
	if( (target - shia.GetPos()).GetLengthSq() < 6.9f )
	{
		// now we are ready to live our lives in the bounded world
		shia.isDoingBoundaryAdjustment = true;
		// enter the charge loop by first doing a Chillout
		return new ChillState( 0.25f );
	}
	return nullptr;
}
void Shia::Charge::Activate( Shia& shia,const World& world )
{
	shia.speed = start_speed;
	// target chili
	shia.SetDirection( (world.GetChiliConst().GetPos() - shia.GetPos()).GetNormalized() );
	shia.spriteIndex = 0;
}

Shia::BrainState* Shia::ChillState::Update( Shia& shia,World& world,float dt )
{
	s_time += dt;
	if( s_time >= duration )
	{
		// stuff for choosing where to charge from
		static std::mt19937 rng( std::random_device{}() );
		std::uniform_int_distribution<int> dist( 0,3 );
		static const Vec2 waypoints[] = {
			{ 150.0f,150.0f },
			{ 650.0f,150.0f },
			{ 150.0f,450.0f },
			{ 650.0f,450.0f }
		};
		// setup the successor sequence which will follow this state
		// (states will execute bottom to top)
		SetSuccessorStates( {
			new ChillState( 0.5f ),
			new Poopin( 1.25f,0.035f,1.0f,7,9.0f ),
			new Wigout( 0.9f,0.025f,0.8f ),
			new Doit,
			new Faceoff,
			new EaseInto( waypoints[dist( rng )],400.0f )
		} );
		return PassTorch();
	}
	return nullptr;
}

void Shia::Faceoff::Activate( Shia& shia,const World& world )
{
	if( shia.GetPos().x > 400.0f )
	{
		shia.facingLeft = true;
	}
	else
	{
		shia.facingLeft = false;
	}
}

Shia::BrainState* Shia::Poopin::Update( Shia& shia,World& world,float dt )
{
	// update state duration timer
	s_time += dt;

	// deliver the scheduled poops
	int poop_count = 0;
	while( iNextPoop != ptimes.cend() && s_time >= *iNextPoop )
	{
		world.SpawnPoo( shia.pos + oe.GetOffset( shia.facingLeft ),
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
		shia.pos = base + Vec2{ shake_dist( rng ),shake_dist( rng ) } * poopin_shake_factor;
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
