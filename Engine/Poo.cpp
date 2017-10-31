#include "Poo.h"
#include "World.h"

Poo::Poo( const Vec2& pos )
	:
	Entity( pos,90.0f,22.0f,8.0f )
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

void Poo::ProcessLogic( const World& world )
{
	// flag for avoidance state
	bool avoiding = false;
	// if close to any enemy, avoid it
	for( auto& other : world.GetPoosConst() )
	{
		// don't consider self
		if( this == &other )
		{
			continue;
		}
		// check if poo is within theshold (hardcoded here as thresh^2)
		const auto delta = GetPos() - other.GetPos();
		const auto lensq = delta.GetLengthSq();
		if( lensq < 400.0f )
		{
			// avoiding state set
			avoiding = true;
			// case for poos at same location
			if( lensq == 0.0f )
			{
				 SetDirection( { -1.0f,1.0f } );
			}
			else
			{
				// normalize delta to get dir (reusing precalculated lensq)
				// if you would have just called Normalize() like a good boy...
				SetDirection( delta / std::sqrt( lensq ) );
			}
			// no need to check other poos
			break;
		}
	}
	// check if in avoidance state, if so do not pursue
	if( !avoiding )
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
}

void Poo::Update( World& world,float dt )
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
			SetReadyForRemoval();
		}
		break;
	}
	// adjust to boundary (crude collision)
	world.GetBoundsConst().Adjust( *this );
}

void Poo::ApplyDamage( float damage )
{
	hp -= int( damage );
	effectState = EffectState::Hit;
	effectTime = 0.0f;
	// play sound effects
	pHitSound->Play( 0.9f,0.3f );
	if( hp <= 0 )
	{
		SetDead();
		pDeathSound->Play( 1.0f,0.8f );
	}
}
