#include "Chili.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "World.h"
#include "ChiliUtil.h"

Chili::Chili( const Vec2& pos,Keyboard& kbd,Mouse& mouse )
	:
	Entity( pos,chiliSpeed,20.0f,18.0f ),
	pKbd( &kbd ),
	pMouse( &mouse )
{
	const auto pLegsSurface = Codex<Surface>::Retrieve( L"Images\\legs-skinny.bmp" );
	// walking animation
	animations.emplace_back( Animation( 32,0,32,33,9,pLegsSurface,0.09f ) );
	// standing animation
	animations.emplace_back( Animation( 0,0,32,33,1,pLegsSurface,10000.0f ) );
}

void Chili::Draw( Graphics& gfx ) const
{
	dshec.DrawChili( gfx );
	dec.DrawChili( gfx );
}

void Chili::ProcessLogic( const World& world )
{
	if( dshec.IsActive() )
	{
		return;
	}
	// aliases to make me life easier
	auto& mouse = *pMouse;
	auto& kbd = *pKbd;
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
		else if(
			e.GetType() == Mouse::Event::Type::RPress &&
			vel != Vec2{ 0.0f,0.0f } &&
			!dshec.IsCooldown() )
		{
			dshec.Activate();
			pDodgeSnd->Play( 1.0f,0.5f );
			return;
		}
	}

	if( !dshec.IsFrozen() )
	{
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
		SetDirection( dir.GetNormalized() );
	}
}

void Chili::SetDirection( const Vec2& dir )
{
	// if moving at all, set animation to walking
	if( dir != Vec2{ 0.0f,0.0f } )
	{
		iCurSequence = AnimationSequence::Walking;
	}
	// completely stationary - standing animation
	else
	{
		iCurSequence = AnimationSequence::Standing;
	}
	// handle velocity and left-right facing
	Entity::SetDirection( dir );
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
	dshec.Update( dt );
}

void Chili::ApplyDamage( float damage )
{
	if( !IsInvincible() )
	{
		health = std::max( 0.0f,health - damage );
	}
	dec.Activate();
}

bool Chili::IsInvincible() const
{
	return dec.IsActive() || dshec.IsActive();
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
			parent.animations[(int)parent.iCurSequence].Draw( legspos,gfx.GetFringeRect(),
				gfx,SpriteEffect::Substitution{ Colors::Magenta,Colors::Red },parent.facingRight
			);
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
				parent.animations[(int)parent.iCurSequence].Draw( legspos,gfx.GetFringeRect(),gfx,
					SpriteEffect::Chroma{ Colors::Magenta },parent.facingRight
				);
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
		parent.animations[(int)parent.iCurSequence].Draw( 
			legspos,gfx.GetFringeRect(),gfx,
			SpriteEffect::Chroma{ Colors::Magenta },parent.facingRight
		);
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

Chili::DashEffectController::DashEffectController( Chili& parent )
	:
	parent( parent )
{}

void Chili::DashEffectController::Update( float dt )
{
	time += dt;
	if( active )
	{
		shadowTime += dt;
		while( shadowTime >= shadowPeriod )
		{
			shadowTime -= shadowPeriod;
			shadows.push_back( { parent.GetPos(),time * kShadowDuration,time * kShadowDuration } );
		}
		if( time >= dashDuration )
		{
			parent.SetSpeed( chiliSpeed );
			parent.SetDirection( { 0.0f,0.0f } );
			active = false;
		}
	}

	if( shadows.size() > 0 )
	{
		for( auto& s : shadows )
		{
			s.t -= dt;
		}
		remove_erase_if( shadows,[]( auto& s ) {return s.t <= 0.0f; } );
	}
}

void Chili::DashEffectController::DrawChili( Graphics& gfx ) const
{
	for( const auto& s : shadows )
	{
		// calculate drawing base
		const auto draw_pos = s.pos + parent.draw_offset;
		// legs offset relative to face
		const auto legspos = Vei2( draw_pos ) + Vei2{ 7,40 };
		// alpha of substituted color
		const auto a = (unsigned char)( 255.0f * (s.t / s.tDeath) );
		// draw legs first (they are behind head)
		parent.animations[(int)parent.iCurSequence].Draw( legspos,gfx.GetFringeRect(),
			gfx,SpriteEffect::SubstitutionBlend{ Colors::Magenta,{ Colors::Black,a } },parent.facingRight
		);
		// draw head
		gfx.DrawSprite( int( draw_pos.x ),int( draw_pos.y ),*parent.pHeadSurface,
			SpriteEffect::SubstitutionBlend{ Colors::Magenta,{ Colors::Black,a } },parent.facingRight
		);
	}
}

void Chili::DashEffectController::Activate()
{
	time = 0.0f;
	shadowTime = shadowPeriod;
	active = true;
	parent.SetSpeed( dashSpeed );
}

bool Chili::DashEffectController::IsActive() const
{
	return active;
}

bool Chili::DashEffectController::IsFrozen() const
{
	return time < freezeTime;
}

bool Chili::DashEffectController::IsCooldown() const
{
	return time < cooldown;
}
