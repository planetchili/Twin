#pragma once

#include "Vec2.h"
#include "SpriteEffect.h"

class Poo
{
public:
	Poo( const Vec2& pos )
		:
		poo( "Images\\poo.bmp" ),
		pos( pos )
	{
	}
	void Draw( Graphics& gfx ) const
	{
		// calculate drawing base
		const auto draw_pos = pos + draw_offset;
		// if effect active, draw sprite replacing opaque pixels with red
		if( effectActive )
		{
			gfx.DrawSprite( int( draw_pos.x ),int( draw_pos.y ),poo,
							SpriteEffect::Substitution{ Colors::White,Colors::White }
			);
		}
		else
		{
			gfx.DrawSprite( int( draw_pos.x ),int( draw_pos.y ),poo,
							SpriteEffect::Chroma{ Colors::White }
			);
		}
	}
	void SetDirection( const Vec2& dir )
	{
		vel = dir * speed;
	}
	void Update( float dt )
	{
		pos += vel * dt;
		// update effect time if active
		if( effectActive )
		{
			effectTime += dt;
			// deactivate effect if duration exceeded
			if( effectTime >= effectDuration )
			{
				effectActive = false;
			}
		}
	}
	void ApplyDamage( float damage )
	{
		hp -= int( damage );
		effectActive = true;
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
private:
	Surface poo;
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
	static constexpr float effectDuration = 0.045f;
	float effectTime = 0.0f;
	bool effectActive = false;
	// hitpoints
	int hp = 100;
};