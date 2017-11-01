#include "Shia.h"
#include "SpriteEffect.h"
#include "World.h"

Shia::Shia( const Vec2& pos )
	:
	Entity( pos,75.0f,90.0f,60.0f )
{}

void Shia::ProcessLogic( const World& world )
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

void Shia::Update( World& world,float dt )
{
	pos += vel * dt;
	world.GetBoundsConst().Adjust( *this );

	if( effectActive )
	{
		effectTime += dt;
		if( effectTime >= effectDuration )
		{
			effectActive = false;
		}
	}
}

void Shia::ApplyDamage( float damage )
{
	effectActive = true;
	effectTime = 0.0f;
}

void Shia::Draw( Graphics& gfx ) const
{
	// calculate drawing base
	const auto draw_pos = Vei2( pos + draw_offset );
	// roomba offset relative to body	
	const auto roomba_pos = Vei2( draw_pos ) + Vei2{ 7,40 };

	if( effectActive ) // draw damage flash
	{
		gfx.DrawSprite( draw_pos.x,draw_pos.y,*pShiaSurf,SpriteEffect::SubstitutionAlpha{ Colors::White },facingLeft );
	}
	else // draw normal
	{
		gfx.DrawSprite( draw_pos.x,draw_pos.y,*pShiaSurf,SpriteEffect::AlphaBlendBaked{},facingLeft );
	}
}
