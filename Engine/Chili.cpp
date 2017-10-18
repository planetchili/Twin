#include "Chili.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "World.h"

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

void Chili::HandleInput( Keyboard& kbd,Mouse& mouse,const World& world )
{
	// process mouse messages while any remain
	while( !mouse.IsEmpty() )
	{
		const auto e = mouse.Read();
		// only interested in left mouse presses
		// fire in the hole! (here is our attack logic)
		if( e.GetType() == Mouse::Event::Type::LPress )
		{
			// bullet spawn location
			bulletSpawnPos = GetPos() + Vec2{ 0.0f,-15.0f };
			// get direction of firing
			bulletDir = (Vec2)e.GetPos() - bulletSpawnPos;
			// process delta to make it direction
			// if delta is 0 set to straight down
			if( bulletDir == Vec2{ 0.0f,0.0f } )
			{
				bulletDir = { 0.0f,1.0f };
			}
			// else normalize
			else
			{
				bulletDir.Normalize();
			}
			// schedule bullet to be spawned
			isFiring = true;
		}
	}
	// process arrow keys state to set direction
	Vec2 dir = { 0.0f,0.0f };
	if( kbd.KeyIsPressed( VK_UP ) )
	{
		dir.y -= 1.0f;
	}
	if( kbd.KeyIsPressed( VK_DOWN ) )
	{
		dir.y += 1.0f;
	}
	if( kbd.KeyIsPressed( VK_LEFT ) )
	{
		dir.x -= 1.0f;
	}
	if( kbd.KeyIsPressed( VK_RIGHT ) )
	{
		dir.x += 1.0f;
	}
	SetDirection( dir );
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

void Chili::ProcessBullet( World& world )
{
	if( isFiring )
	{
		isFiring = false;
		world.SpawnBullet( { bulletSpawnPos,bulletDir } );
	}
}

void Chili::Update( World& world,float dt )
{
	pos += vel * dt;
	// adjust chili to boundary
	world.GetBoundsConst().Adjust( *this );
	// process bullet
	ProcessBullet( world );
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
