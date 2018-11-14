#include "Shia.h"
#include "SpriteEffect.h"
#include "World.h"
#include "BvShiaSlowRoll.h"
#include "SpriteElement.h"
#include "BvShiaDecide.h"
#include "BvShiaChill.h"
#include "BvShiaChasedown.h"
#include "BvShiaOrbit.h"

Shia::Shia( const Vec2& pos )
	:
	Entity( pos,75.0f,90.0f,60.0f ),
	pBehavior( new SlowRoll( *this,{ 368.0f,300.0f } ) ) // (1st)
{
	// last state on stack is decide (4th)
	pBehavior->PushSuccessorState( new Decide( rng ) );
	// orbit and chasedown (3rd)
	pBehavior->PushSuccessorStates( {
		new Chill( 0.5f ),
		new Chasedown( 4.5f ),
		new Chill( 0.5f ),
		new Orbit( rng ),
	} );
	// start sequence 2 poo 2 charge (2nd)
	pBehavior->PushSuccessorStates( Decide::MakeChargeSequence( rng ) );
	pBehavior->PushSuccessorStates( Decide::MakeChargeSequence( rng ) );
	pBehavior->PushSuccessorStates( Decide::MakePoopinSequence( rng ) );
	pBehavior->PushSuccessorStates( Decide::MakePoopinSequence( rng ) );
}

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
	if( ultimate.IsActive() )
	{
		ultimate.Update( *this,dt );
	}
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
		sprite.Draw( pos,gfx.GetFringeRect(),gfx,
			SpriteEffect::AlphaBakedSubstitution{ { Colors::White,0x0B } },facingRight );
	}
	else // draw normal
	{
		sprite.Draw( pos,gfx.GetFringeRect(),gfx,
			SpriteEffect::AlphaBlendBaked{},facingRight );
	}
}

void Shia::DisplaceBy( const Vec2& d )
{
	// bounce off walls
	// (right now, displace is only used when we hit a wall)
	// if d.x != 0, then we know that we hit a vertical wall etc.
	if( d.x != 0.0f )
	{
		vel.x = -vel.x;
	}
	if( d.y != 0.0f )
	{
		vel.y = -vel.y;
	}
	// do the actual position displacement
	Entity::DisplaceBy( d );
}

Shia::Sprite::Sprite()
	:
	SpriteControl( 
[]( SpriteMode mode ) -> SpriteElement*
{
	switch( mode )
	{
	case SpriteMode::Standing:
		return new CompositeSpriteElement( {
			new SurfaceSpriteElement( Codex<Surface>::Retrieve( L"Images\\pm_roomba_left.png" ),
				{ -47.0f,-33.0f },{ -47.0f,-33.0f }
			),
			new SurfaceSpriteElement( Codex<Surface>::Retrieve( L"Images\\pm_shia_left.png" ),
				{ -26.0f,-161.0f },{ -26.0f,-161.0f }
			)
		} );
	case SpriteMode::Pooping:
		return new CompositeSpriteElement( {
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
	case SpriteMode::Beam:
		return new CompositeSpriteElement( {
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
	default:
		assert( "Bad Mode in Sprite Element factory functor!" && false );
		return nullptr;
	}
},SpriteMode::Standing )
{}

Shia::Ultimate::Ultimate()
	:
	OffsetElement( { -28.0f,-65.0f },{ 28.0f,-65.0f } )
{}

void Shia::Ultimate::Draw( const Shia& shia,Graphics& gfx ) const
{
	if( IsBeaming() )
	{
		const Vec2 off = shia.pos + GetOffset( shia.facingRight );
		for( const auto& b : beams )
		{
			// draw triangle
			gfx.DrawTriangle( b.t.v0,b.t.v1,b.t.v2,
				gfx.GetFringeRect(),
			[origin = off,alpha_base = clamp( ldist( rng ),0.0f,1.0f ),b,src = color]
			( int x,int y,Graphics& gfx ) {
				// load dst pixel
				const auto dst = gfx.GetPixel( x,y );
				// calculate alpha
				const float inline_factor = (Vec2( (float)x,(float)y ) - origin).GetNormalized() * b.centerDir;
				const float alpha = (inline_factor - 0.97f) * 33.0f * alpha_base;
				const unsigned int alpha_int = unsigned int( 255.0f * alpha );
				const unsigned int calpha_int = 255u - alpha_int;
				// scale src color
				const unsigned int src_rb = (((src.dword & 0xFF00FFu) * alpha_int) >> 8) & 0xFF00FFu;
				const unsigned int src_g = (((src.dword & 0x00FF00u) * alpha_int) >> 8) & 0x00FF00u;
				// scale dst color
				const unsigned int dst_rb = (((dst.dword & 0xFF00FFu) * calpha_int) >> 8) & 0xFF00FFu;
				const unsigned int dst_g = (((dst.dword & 0x00FF00u) * calpha_int) >> 8) & 0x00FF00u;
				// blend and write
				gfx.PutPixel( x,y,dst_rb + dst_g + src_rb + src_g + 0x20000000u );
			});
		}
	}
}

void Shia::Ultimate::Update( const Shia& shia,float dt )
{
	theta += rotationSpeed * dt;
	t += dt;
	GenerateTriangles( shia );
}

bool Shia::Ultimate::IsCollidingWith( const RectF& rect ) const
{
	return std::any_of( beams.begin(),beams.end(),[rect]( const auto& b )
	{
		return b.t.Overlaps( rect );
	} );
}

void Shia::Ultimate::GenerateTriangles( const Shia& shia )
{
	// generate 3 vertex positions of base triangle for effect
	const Vec2 center = { 0.0f,0.0f };
	const Vec2 left = Vec2{ 0.0f,-length }.GetRotated( -width / 2.0f );
	const Vec2 right = Vec2{ 0.0f,-length }.GetRotated( width / 2.0f );
	// calculate offset (origin on screen/world)
	const Vec2 off = shia.pos + GetOffset( shia.facingRight );
	// shader to blend a constant color with backbuffer
	// draw the triangles
	for( int i = 0; i < nBeams; i++ )
	{
		// calculate rotated vertices/dir
		const float baseAngle = float( i ) * separation;
		const Vec2 left_rotated = left.GetRotated( baseAngle + theta );
		const Vec2 right_rotated = right.GetRotated( baseAngle + theta );
		const Vec2 center_dir = (left_rotated + right_rotated).GetNormalized();
		beams[i] = { { center + off,left_rotated + off,right_rotated + off },center_dir };
	}
}
