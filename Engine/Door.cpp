#include "Door.h"
#include "World.h"

Door::Door( const Vec2& pos )
	:
	Entity( pos,90.0f,22.0f,8.0f ),
	pBehavior( new Coast )
{
}

void Door::Draw( Graphics& gfx ) const
{
	gfx.DrawSprite( int( pos.x - halfSeparation ),int( pos.y ),*pLeft,
		SpriteEffect::AlphaBlendBaked{}
	);
	gfx.DrawSprite( int( pos.x + halfSeparation ),int( pos.y ),*pRight,
		SpriteEffect::AlphaBlendBaked{}
	);
}

void Door::ProcessLogic( const World& world )
{}

void Door::Update( World& world,float dt )
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
