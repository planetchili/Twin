#include "Poo.h"

Poo::Poo( const Vec2& pos )
	:
	pos( pos )
{}

void Poo::Draw( Graphics& gfx ) const
{
	// calculate drawing base
	const auto draw_pos = pos + draw_offset;
	// switch on effectState to determine drawing method
	switch( effectState )
	{
	case EffectState::Hit:
		// flash white for hit
		gfx.DrawSprite( int( draw_pos.x ),int( draw_pos.y ),*pPooSurface,
			SpriteEffect::Substitution{ Colors::White,Colors::White }
		);
		break;
	case EffectState::Dying:
		// draw dissolve effect during dying (tint red)
		gfx.DrawSprite( int( draw_pos.x ),int( draw_pos.y ),*pPooSurface,
			SpriteEffect::DissolveHalfTint{ Colors::White,Colors::Red,
			1.0f - effectTime / dissolveDuration }
		);
		break;
	case EffectState::Normal:
		gfx.DrawSprite( int( draw_pos.x ),int( draw_pos.y ),*pPooSurface,
			SpriteEffect::Chroma{ Colors::White }
		);
		break;
	}
}

void Poo::SetDirection( const Vec2 & dir )
{
	vel = dir * speed;
}

void Poo::Update( float dt )
{
	// dead poos tell no tales (or even move for that matter)
	if( !IsDead() )
	{
		pos += vel * dt;
	}

	// always update effect time (who cares brah?)
	effectTime += dt;
	// effect state machine logic
	switch( effectState )
	{
	case EffectState::Hit:
		if( effectTime >= hitFlashDuration )
		{
			// if we are dead, transition to dying dissolve state
			if( IsDead() )
			{
				effectState = EffectState::Dying;
				effectTime = 0.0f;
			}
			else
			{
				effectState = EffectState::Normal;
			}
		}
		break;
	case EffectState::Dying:
		if( effectTime >= dissolveDuration )
		{
			// dissolve finished, get this traysh outta ere!
			isReadyForRemoval = true;
		}
		break;
	}
}

void Poo::ApplyDamage( float damage )
{
	hp -= int( damage );
	effectState = EffectState::Hit;
	effectTime = 0.0f;
	// play sound effects
	pHitSound->Play( 0.9f,0.3f );
	if( IsDead() )
	{
		pDeathSound->Play( 1.0f,0.8f );
	}
}

const Vec2& Poo::GetPos() const
{
	return pos;
}

RectF Poo::GetHitbox() const
{
	return RectF::FromCenter( pos,hitbox_halfwidth,hitbox_halfheight );
}

bool Poo::IsDead() const
{
	return hp <= 0;
}

bool Poo::IsReadyForRemoval() const
{
	return isReadyForRemoval;
}

void Poo::DisplaceBy( const Vec2& d )
{
	pos += d;
}
