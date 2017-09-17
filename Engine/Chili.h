#pragma once

#include "Animation.h"
#include "Vec2.h"
#include "SpriteEffect.h"

class Chili
{
private:
	enum class Sequence
	{
		Walking,
		Standing,
		Count
	};
public:
	Chili( const Vec2& pos )
		:
		head( "Images\\chilihead.bmp" ),
		legs( "Images\\legs-skinny.bmp" ),
		pos( pos )
	{
		// walking animation
		animations.emplace_back( Animation( 32,0,32,33,9,legs,0.12f ) );
		// standing animation
		animations.emplace_back( Animation( 0,0,32,33,1,legs,10000.0f ) );
	}
	void Draw( Graphics& gfx ) const
	{
		// if effect active, draw sprite replacing opaque pixels with red
		if( effectActive )
		{
			// legs offset relative to face
			const auto legspos = Vei2( pos ) + Vei2{ 7,40 };
			// draw legs first (they are behind head)
			animations[(int)iCurSequence].DrawColor( legspos,gfx,Colors::Red,facingRight );
			// draw head
			gfx.DrawSprite( int( pos.x ),int( pos.y ),head,
							SpriteEffect::Substitution{ Colors::Magenta,Colors::Red },
							facingRight
			);
		}
		else
		{
			// legs offset relative to face
			const auto legspos = Vei2( pos ) + Vei2{ 7,40 };
			// draw legs first (they are behind head)
			animations[(int)iCurSequence].Draw( legspos,gfx,facingRight );
			// draw head
			gfx.DrawSprite( int( pos.x ),int( pos.y ),head,
							SpriteEffect::Chroma{ Colors::Magenta },
							facingRight
			);
		}
	}
	void SetDirection( const Vec2& dir )
	{
		// x vel determines direction
		if( dir.x > 0.0f )
		{
			iCurSequence = Sequence::Walking;
			facingRight = true;
		}
		else if( dir.x < 0.0f )
		{
			iCurSequence = Sequence::Walking;
			facingRight = false;
		}
		// else if x stationary but moving in y
		// keep direction, just make sure animation is walking
		else if( dir.y != 0.0f )
		{
			iCurSequence = Sequence::Walking;
		}
		// completely stationary
		else
		{
			// direction remains same as last moving dir
			// just set animation
			iCurSequence = Sequence::Standing;
		}
		vel = dir * speed;
	}
	void Update( float dt )
	{
		pos += vel * dt;
		animations[(int)iCurSequence].Update( dt );
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
	void ActivateEffect()
	{
		effectActive = true;
		effectTime = 0.0f;
	}
	const Vec2& GetPos() const
	{
		return pos;
	}
private:
	Surface head;
	Surface legs;
	Vec2 pos;
	Vec2 vel = { 0.0f,0.0f };
	std::vector<Animation> animations;
	Sequence iCurSequence = Sequence::Standing;
	// used to keep track of graphical facing (for sprite mirroring)
	bool facingRight = true;
	float speed = 110.0f;
	static constexpr float effectDuration = 0.045f;
	float effectTime = 0.0f;
	bool effectActive = false;
};