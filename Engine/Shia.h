#pragma once

#include "Vec2.h"
#include "Rect.h"
#include "Codex.h"
#include "Entity.h"
#include "Sound.h"
#include "SpriteControl.h"

class Shia : public Entity
{
	// sprite modes
	enum class SpriteMode
	{
		Standing,
		Pooping,
		Beam,
		Count
	};
	// chili graphics control
	class Sprite : public SpriteControl<SpriteMode>
	{
	public:
		Sprite();
	};

	// behavior parent class
	using Behavior = Entity::Behavior<Shia>;
	// slowly move to target position
	class SlowRoll;
	// just chill out where you be at
	class Chill;
	// quadradit speedup slowdown towards target pos
	class EaseInto;
	// charge at chili w/ impulse and drag, bounce off walls
	class Charge;
	// vibrate crazily (used for pre-charge)
	class Wigout;
	// shake-n poop
	class Poopin;
	// pass-through just doit! sound cue
	class Doit;

public:
	Shia( const Vec2& pos );
	// here the Shia does it's 'thinking' and decides its actions
	void ProcessLogic( const class World& world ) override;
	// here the Shia updates physical state based on the dt and the world
	void Update( class World& world,float dt ) override;
	void ApplyDamage( float damage ) override;
	void Draw( Graphics& gfx ) const override;
	void DisplaceBy( const Vec2& d )
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
private:
	// sprite graphics component/controller/whatever
	Sprite sprite;
	// hit flash effect shizzle
	// (should probably be moved into Sprite right?)
	bool effectActive = false;
	float effectTime = 0.0f;
	float effectDuration = 0.045f;
	// we don't wanna do boundary adjustment during our big entrance!
	bool isDoingBoundaryAdjustment;
	// this is the state that holds our logic
	Behavior* pBehavior;
	// sounds
	const Sound* poop_sound = Codex<Sound>::Retrieve( L"Sounds\\fart2.wav" );
	const Sound* doit_sound1 = Codex<Sound>::Retrieve( L"Sounds\\just_doit1.mp3" );
};