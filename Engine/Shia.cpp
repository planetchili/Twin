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
			{ 0.13f,0.13f,0.13f,0.13f,0.18f,std::numeric_limits<float>::max() },
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

	spritePtrs[spriteIndex]->Update( dt );
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
		spritePtrs[spriteIndex]->SetEffectColor( Colors::White );
		spritePtrs[spriteIndex]->Draw( pos,gfx.GetScreenRect(),gfx,SpriteElement::Effect::ColorSub,facingLeft );
	}
	else // draw normal
	{
		spritePtrs[spriteIndex]->Draw( pos,gfx.GetScreenRect(),gfx,SpriteElement::Effect::None,facingLeft );
	}
	gfx.DrawRectThin( RectI( GetHitbox() ),Colors::Green );
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

Shia::BrainState* Shia::SlowRollState::Update( Shia& shia,const World& world,float dt )
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

Shia::BrainState* Shia::ChillState::Update( Shia& shia,const World& world,float dt )
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
			new Charge( 1000.0f,40.0f,250.0f ),
			new Wigout( 1.0f,0.025f,1.75f ),
			new Faceoff,
			new EaseInto( waypoints[dist( rng )],400.0f )
		} );
		return PassTorch();
	}
	return nullptr;
}

void Shia::Faceoff::Activate( Shia& shia,const World& world )
{
	if( shia.GetPos().x > world.GetChiliConst().GetPos().x )
	{
		shia.facingLeft = true;
	}
	else
	{
		shia.facingLeft = false;
	}
}
