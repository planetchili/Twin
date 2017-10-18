#include "Chili.h"

Chili::Chili( const Vec2& pos )
	:
	pos( pos )
{
	const auto pLegsSurface = Codex<Surface>::Retrieve( L"Images\\legs-skinny.bmp" );
	// walking animation
	animations.emplace_back( Animation( 32,0,32,33,9,pLegsSurface,0.09f ) );
	// standing animation
	animations.emplace_back( Animation( 0,0,32,33,1,pLegsSurface,10000.0f ) );
}

void Chili::Draw( Graphics& gfx ) const
{
	dec.DrawChili( gfx );
}

void Chili::SetDirection( const Vec2& dir )
{
	// x vel determines direction
	if( dir.x > 0.0f )
	{
		iCurSequence = AnimationSequence::Walking;
		facingRight = true;
	}
	else if( dir.x < 0.0f )
	{
		iCurSequence = AnimationSequence::Walking;
		facingRight = false;
	}
	// else if x stationary but moving in y
	// keep direction, just make sure animation is walking
	else if( dir.y != 0.0f )
	{
		iCurSequence = AnimationSequence::Walking;
	}
	// completely stationary
	else
	{
		// direction remains same as last moving dir
		// just set animation
		iCurSequence = AnimationSequence::Standing;
	}
	vel = dir * speed;
}

void Chili::Update( float dt )
{
	pos += vel * dt;
	animations[(int)iCurSequence].Update( dt );
	// update the damage effect controller
	dec.Update( dt );
}

void Chili::ApplyDamage()
{
	dec.Activate();
}

const Vec2& Chili::GetPos() const
{
	return pos;
}

RectF Chili::GetHitbox() const
{
	return RectF::FromCenter( pos,hitbox_halfwidth,hitbox_halfheight );
}

bool Chili::IsInvincible() const
{
	return dec.IsActive();
}

void Chili::DisplaceBy( const Vec2& d )
{
	pos += d;
}

Chili::DamageEffectController::DamageEffectController( Chili& parent )
	:
	parent( parent )
{}

void Chili::DamageEffectController::Update( float dt )
{
	// update effect time if active
	if( active )
	{
		time += dt;
		// deactivate effect if duration exceeded
		if( time >= totalDuration )
		{
			active = false;
		}
	}
}

void Chili::DamageEffectController::DrawChili( Graphics& gfx ) const
{
	// calculate drawing base
	const auto draw_pos = parent.pos + parent.draw_offset;
	// legs offset relative to face
	const auto legspos = Vei2( draw_pos ) + Vei2{ 7,40 };

	// if effect active, draw sprite based on effect
	if( active )
	{
		// red flash for first instant
		if( time <= RedDuration )
		{
			// draw legs first (they are behind head)
			parent.animations[(int)parent.iCurSequence].DrawColor(
				legspos,gfx,Colors::Red,parent.facingRight );
			// draw head
			gfx.DrawSprite( int( draw_pos.x ),int( draw_pos.y ),*parent.pHeadSurface,
				SpriteEffect::Substitution{ Colors::Magenta,Colors::Red },
				parent.facingRight
			);
		}
		// after that, blink
		else
		{
			// divide time by half-period of blink
			// even multiples will be blinking out
			if( int( time / blinkHalfPeriod ) % 2 != 0 )
			{
				// draw legs first (they are behind head)
				parent.animations[(int)parent.iCurSequence].Draw( legspos,gfx,parent.facingRight );
				// draw head
				gfx.DrawSprite( int( draw_pos.x ),int( draw_pos.y ),*parent.pHeadSurface,
					SpriteEffect::Chroma{ Colors::Magenta },
					parent.facingRight
				);
			}
		}
	}
	// effect not active, draw normally
	else
	{
		// draw legs first (they are behind head)
		parent.animations[(int)parent.iCurSequence].Draw( legspos,gfx,parent.facingRight );
		// draw head
		gfx.DrawSprite( int( draw_pos.x ),int( draw_pos.y ),*parent.pHeadSurface,
			SpriteEffect::Chroma{ Colors::Magenta },
			parent.facingRight
		);
	}
}

void Chili::DamageEffectController::Activate()
{
	if( !active )
	{
		active = true;
		time = 0.0f;
		parent.pHurtSfx->Play();
	}
}

bool Chili::DamageEffectController::IsActive() const
{
	return active;
}
