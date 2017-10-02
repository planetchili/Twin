#pragma once

#include "Vec2.h"
#include "SpriteEffect.h"
#include "Codex.h"

class Poo
{
private:
	enum class EffectState
	{
		Normal,
		Hit,
		Dying
	};
public:
	Poo( const Vec2& pos )
		:
		pPooSurface( Codex<Surface>::Retrieve( L"Images\\poo.bmp" ) ),
		pos( pos )
	{
	}
	void Draw( Graphics& gfx ) const
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
	void SetDirection( const Vec2& dir )
	{
		vel = dir * speed;
	}
	void Update( float dt )
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
	void ApplyDamage( float damage )
	{
		hp -= int( damage );
		effectState = EffectState::Hit;
		effectTime = 0.0f;
	}
	const Vec2& GetPos() const
	{
		return pos;
	}
	RectF GetHitbox() const
	{
		return RectF::FromCenter( pos,hitbox_halfwidth,hitbox_halfheight );
	}
	bool IsDead() const
	{
		return hp <= 0;
	}
	bool IsReadyForRemoval() const
	{
		return isReadyForRemoval;
	}
	void DisplaceBy( const Vec2& d )
	{
		pos += d;
	}
private:
	const Surface* pPooSurface;
	Vec2 pos;
	// hitbox dimensions
	float hitbox_halfwidth = 11.0f;
	float hitbox_halfheight = 4.0f;
	// this value give the offset from the actual base of the
	// character to its drawing base
	Vec2 draw_offset = { -11.0f,-19.0f };
	Vec2 vel = { 0.0f,0.0f };
	// used to keep track of graphical facing (for sprite mirroring)
	float speed = 90.0f;
	static constexpr float dissolveDuration = 0.6f;
	static constexpr float hitFlashDuration = 0.045f;
	float effectTime = 0.0f;
	EffectState effectState = EffectState::Normal;
	bool isReadyForRemoval = false;
	// hitpoints
	int hp = 100;
};