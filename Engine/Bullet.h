#pragma once

#include "Vec2.h"
#include "Animation.h"
#include "SpriteEffect.h"

class Bullet
{
public:
	Bullet( const Vec2& pos,const Vec2& dir )
		:
		bullet( "Images\\fireball.bmp" ),
		bullet_animation( 0,0,8,8,4,bullet,0.1f ),
		pos( pos ),
		vel( dir * speed )
	{}
	void Draw( Graphics& gfx ) const
	{
		// calculate drawing base
		const auto draw_pos = pos + draw_offset;
		// draw the bullet
		bullet_animation.Draw( (Vei2)draw_pos,gfx );
	}
	void Update( float dt )
	{
		pos += vel * dt;
		bullet_animation.Update( dt );
	}
	const Vec2& GetPos() const
	{
		return pos;
	}
	RectF GetHitbox() const
	{
		return RectF::FromCenter( pos,hitbox_halfwidth,hitbox_halfheight );
	}
	Bullet( const Bullet& src )
		:
		bullet( src.bullet ),
		bullet_animation( 0,0,8,8,4,bullet,0.1f ),
		pos( src.pos ),
		vel( src.vel )
	{}
	Bullet& operator=( const Bullet& rhs )
	{
		pos = rhs.pos;
		vel = rhs.vel;
		return *this;
	}
private:
	Surface bullet;
	Animation bullet_animation;
	Vec2 pos;
	// hitbox dimensions
	float hitbox_halfwidth = 4.0f;
	float hitbox_halfheight = 4.0f;
	// bullet speed
	float speed = 300.0f;
	// this value give the offset from the actual base of the
	// character to its drawing base
	Vec2 draw_offset = { -4.0f,-4.0f };
	Vec2 vel = { 0.0f,0.0f };
};