#pragma once

#include "Animation.h"
#include "Vec2.h"
#include "SpriteEffect.h"

class Chili
{
private:
	class DamageEffectController
	{
	public:
		DamageEffectController( Chili& parent )
			:
			parent( parent )
		{}
		// update damage effect time
		void Update( float dt )
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
		// draw chili based on damage effect state
		void DrawChili( Graphics& gfx ) const
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
					gfx.DrawSprite( int( draw_pos.x ),int( draw_pos.y ),parent.head,
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
						gfx.DrawSprite( int( draw_pos.x ),int( draw_pos.y ),parent.head,
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
				gfx.DrawSprite( int( draw_pos.x ),int( draw_pos.y ),parent.head,
								SpriteEffect::Chroma{ Colors::Magenta },
								parent.facingRight
				);
			}
		}
		// activate damage effect
		void Activate()
		{
			if( !active )
			{
				active = true;
				time = 0.0f;
			}
		}
	private:
		Chili& parent;
		static constexpr float RedDuration = 0.045f;
		static constexpr float totalDuration = 1.5f;
		static constexpr float blinkHalfPeriod = 0.18f;
		float time;
		bool active = false;
	};
	enum class AnimationSequence
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
		dec.DrawChili( gfx );
	}
	void SetDirection( const Vec2& dir )
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
	void Update( float dt )
	{
		pos += vel * dt;
		animations[(int)iCurSequence].Update( dt );
		// update the damage effect controller
		dec.Update( dt );
	}
	void ActivateEffect()
	{
		dec.Activate();
	}
	const Vec2& GetPos() const
	{
		return pos;
	}
	RectF GetHitbox() const
	{
		return RectF::FromCenter( pos,hitbox_halfwidth,hitbox_halfheight );
	}
private:
	Surface head;
	Surface legs;
	Vec2 pos;
	// hitbox dimensions
	float hitbox_halfwidth = 10.0f;
	float hitbox_halfheight = 9.0f;
	// this value give the offset from the actual base of the
	// character to its drawing base
	Vec2 draw_offset = { -21.0f,-67.0f };
	Vec2 vel = { 0.0f,0.0f };
	std::vector<Animation> animations;
	AnimationSequence iCurSequence = AnimationSequence::Standing;
	// used to keep track of graphical facing (for sprite mirroring)
	bool facingRight = true;
	float speed = 110.0f;
	DamageEffectController dec = { *this };
};