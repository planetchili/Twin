#include "Shia.h"
#include "SpriteEffect.h"
#include "World.h"

Shia::Shia( const Vec2& pos )
	:
	pos( pos )
{}

void Shia::ProcessLogic( const World & world )
{
	const auto delta = world.GetChiliConst().GetPos() - GetPos();
	// we only wanna move if not already really close to target pos
	// (prevents vibrating around target point; 3.0 just a number pulled out of butt)
	if( delta.GetLengthSq() > 3.0f )
	{
		SetDirection( delta.GetNormalized() );
	}
	else
	{
		SetDirection( { 0.0f,0.0f } );
	}
}

void Shia::Update( const World& world,float dt )
{
	pos += vel * dt;
	world.GetBoundsConst().Adjust( *this );
}

void Shia::ApplyDamage( float damage )
{}

const Vec2 & Shia::GetPos() const
{
	return pos;
}

RectF Shia::GetHitbox() const
{
	return RectF::FromCenter( pos,hitbox_halfwidth,hitbox_halfheight );
}

bool Shia::IsDead() const
{
	return false;
}

bool Shia::IsReadyForRemoval() const
{
	return false;
}

void Shia::DisplaceBy( const Vec2& d )
{
	pos += d;
}

void Shia::Draw( Graphics& gfx ) const
{
	// calculate drawing base
	const auto draw_pos = Vei2( pos + draw_offset );
	// roomba offset relative to body	
	const auto roomba_pos = Vei2( draw_pos ) + Vei2{ 7,40 };

	const auto ae = SpriteEffect::AlphaBlendBaked{};
	// draw shia
	gfx.DrawSprite( draw_pos.x,draw_pos.y,*pShiaSurf,ae,!facingRight );
}

void Shia::SetDirection( const Vec2& dir )
{
	if( dir.x > 0.0f )
	{
		facingRight = true;
	}
	else if( dir.x < 0.0f )
	{
		facingRight = false;
	}
	vel = dir * speed;
}
