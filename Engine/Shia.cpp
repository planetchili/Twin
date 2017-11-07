#include "Shia.h"
#include "SpriteEffect.h"
#include "World.h"
#include "BvShiaSlowRoll.h"

Shia::Shia( const Vec2& pos )
	:
	Entity( pos,75.0f,90.0f,60.0f ),
	pBehavior( new SlowRoll( *this,{ 368.0f,300.0f } ) )
{}

void Shia::ProcessLogic( const World& world )
{}

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
	while( auto pNewState = pBehavior->Update( *this,world,dt ) )
	{
		delete pBehavior;
		pBehavior = pNewState;
		pBehavior->Activate( *this,world );
	}

	sprite.Update( dt );
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
		sprite.Draw( pos,gfx.GetScreenRect(),gfx,
			SpriteEffect::AlphaBakedSubstitution{ Colors::White },facingRight );
	}
	else // draw normal
	{
		sprite.Draw( pos,gfx.GetScreenRect(),gfx,
			SpriteEffect::AlphaBlendBaked{},facingRight );
	}
}

void Shia::Charge::Activate( Shia& shia,const World& world )
{
	shia.speed = start_speed;
	// target chili
	shia.SetDirection( (world.GetChiliConst().GetPos() - shia.GetPos()).GetNormalized() );
	shia.sprite.SetMode( Sprite::Mode::Standing );
}

Shia::Behavior* Shia::ChillState::Update( Shia& shia,World& world,float dt )
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
			new EaseInto( waypoints[dist( rng )],400.0f )
		} );
		return PassTorch();
	}

	shia.SetDirection( { 0.0f,0.0f } );
	return nullptr;
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

Shia::Sprite::Sprite()
	:
	elementPtrs( (int)Mode::Count )
{
	elementPtrs[(int)Mode::Standing] = new CompositeSpriteElement( {
		new SurfaceSpriteElement( Codex<Surface>::Retrieve( L"Images\\pm_roomba_left.png" ),
			{ -47.0f,-33.0f },{ -47.0f,-33.0f }
		),
		new SurfaceSpriteElement( Codex<Surface>::Retrieve( L"Images\\pm_shia_left.png" ),
			{ -26.0f,-161.0f },{ -26.0f,-161.0f }
		)
	} );
	elementPtrs[(int)Mode::Pooping] = new CompositeSpriteElement( {
		new SurfaceSpriteElement( Codex<Surface>::Retrieve( L"Images\\pm_roomba_left.png" ),
			{ -47.0f,-33.0f },{ -47.0f,-33.0f }
		),
		new AnimationSpriteElement(
			0,0,99,154,6,
			Codex<Surface>::Retrieve( L"Images\\pm_shia_poopin.png" ),
			{ 0.11f,0.11f,0.11f,0.11f,0.18f,std::numeric_limits<float>::max() },
			{ -66.0f,-160.0f },
			{ -35.0f,-160.0f }
		)
	} );
	elementPtrs[(int)Mode::Beam] = new CompositeSpriteElement( {
		new SurfaceSpriteElement( Codex<Surface>::Retrieve( L"Images\\pm_roomba_left.png" ),
			{ -47.0f,-33.0f },{ -47.0f,-33.0f }
		),
		new AnimationSpriteElement(
			0,0,120,160,11,
			Codex<Surface>::Retrieve( L"Images\\pm_shia_beam.png" ),
			{ 0.25f,0.25f,0.25f,0.25f,0.25f,0.25f,0.25f,0.25f,0.25f,0.25f,
			std::numeric_limits<float>::max() },
			{ -66.0f,-160.0f },
			{ -53.0f,-160.0f }
		)
	} );
	// make sure all slots in element vector were initialized
	assert( std::none_of( elementPtrs.begin(),elementPtrs.end(),[]( auto p ) { return p != nullptr; } ) );
}

Shia::Sprite::~Sprite()
{
	for( auto p : elementPtrs )
	{
		delete p;
	}
}

void Shia::Sprite::SetMode( Mode newMode )
{
	curMode = newMode;
}

void Shia::Sprite::Reset()
{
	GetCurrentElement().Reset();
}

void Shia::Sprite::Update( float dt )
{
	GetCurrentElement().Update( dt );
}

void Shia::Sprite::Draw( const Vec2& pos,const RectI& clip,Graphics& gfx,const SpriteEffect::Driver& effect,bool mirrored ) const
{
	GetCurrentElement().Draw( pos,clip,gfx,effect,mirrored );
}

SpriteElement& Shia::Sprite::GetCurrentElement()
{
	return *elementPtrs[(int)curMode];
}

const SpriteElement& Shia::Sprite::GetCurrentElement() const
{
	return const_cast<Sprite*>(this)->GetCurrentElement();
}
