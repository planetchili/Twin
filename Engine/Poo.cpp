#include "Poo.h"
#include "World.h"
#include "BvPooCoast.h"
#include "BvPooCharge.h"
#include "BvPooVibrate.h"
#include "BvPooPursue.h"
#include <typeinfo>

Poo::Poo( const Vec2& pos,const Vec2& vel_in )
	:
	Entity( pos,90.0f,22.0f,8.0f ),
	pBehavior( new Coast ) // start off coasting w/ velocity
{
	vel = vel_in;
	speed = vel.GetLength();
	pBehavior->SetSuccessorStates( { new Vibrate } );
}

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

	// adjust to boundary (crude collision)
	world.GetBoundsConst().Adjust( *this );
}

void Poo::ApplyDamage( float damage )
{
	hp -= int( damage );
	effectState = EffectState::Hit;
	effectTime = 0.0f;
	if( hp <= 0 )
	{
		SetDead();
		pDeathSound->Play( 1.0f,0.8f );
	}
}

void Poo::DisplaceBy( const Vec2 & d )
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

void Poo::Fling()
{
	assert( typeid(*pBehavior) == typeid(Coast) && vel.GetLengthSq() == 0.0f );
	delete pBehavior;
	pBehavior = new Vibrate;
	pBehavior->SetSuccessorStates( {
		new Coast,
		new Charge
	} );
}

void Poo::Awaken()
{
	assert( typeid(*pBehavior) == typeid(Coast) && vel.GetLengthSq() == 0.0f );
	delete pBehavior;
	pBehavior = new Vibrate;
	pBehavior->SetSuccessorStates( {
		new Coast,
		new Pursue
	} );
}
