#pragma once

#include "Vec2.h"
#include "Rect.h"
#include "Codex.h"
#include "Entity.h"
#include "Sound.h"
#include "SpriteControl.h"
#include "ChiliMath.h"
#include <random>
#include "FrameTimer.h"
#include "Triangle.h"

class Shia : public Entity
{
	// ultimate attack graphics control
	class Ultimate : private OffsetElement
	{
	private:
		struct Beam
		{
			Triangle<Vec2> t;
			Vec2 centerDir;
		};
	public:
		Ultimate();
		void Draw( const Shia& shia,class Graphics& gfx ) const;
		void Update( const Shia& shia,float dt );
		void Activate()
		{
			active = true;
		}
		bool IsActive() const
		{
			return active;
		}
		void Reset()
		{
			t = 0.0f;
			theta = 0.0f;
			active = false;
		}
		bool IsBeaming() const
		{
			return t >= activeTime;
		}
		bool IsCollidingWith( const RectF& rect ) const;
	private:
		void GenerateTriangles( const Shia& shia );
	private:
		static constexpr float width = PI / 8.0f;
		static constexpr float length = 600.0f;
		static constexpr int nBeams = 5;
		static constexpr float separation = 2.0f * PI / float( nBeams );
		static constexpr Color color = Colors::White;
		static constexpr float base_alpha = 0.65f;
		static constexpr float activeTime = 2.7f;
		std::vector<Beam> beams = std::vector<Beam>{ (size_t)nBeams };
		mutable std::mt19937 rng = std::mt19937( std::random_device{}() );
		mutable std::normal_distribution<float> ldist =
			std::normal_distribution<float>( base_alpha,0.2f );
		float t = 0.0f;
		float theta = 0.0f;
		float rotationSpeed = PI / 4.2f;
		bool active = false;
	};

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
	// stand still with beam attack
	class Beamer;
	// decide next attack pattern
	class Decide;
	// chase chili
	class Chasedown;
	// fling a poo
	class Fling;
	// awaken some poo
	class Awaken;
	// defensive poos orbit shia
	class Orbit;
	// block state machine until certain time
	class WaitUntil;

public:
	Shia( const Vec2& pos );
	// here the Shia does it's 'thinking' and decides its actions
	void ProcessLogic( const class World& world ) override;
	// here the Shia updates physical state based on the dt and the world
	void Update( class World& world,float dt ) override;
	void ApplyDamage( float damage ) override;
	void Draw( Graphics& gfx ) const override;
	// this is called when we hit the walls
	void DisplaceBy( const Vec2& d );
	// access the ultimate
	Ultimate& GetUltimate()
	{
		return ultimate;
	}
	const Ultimate& GetUltimate() const
	{
		return const_cast<Shia*>(this)->GetUltimate();
	}
private:
	// sprite graphics component/controller/whatever
	Sprite sprite;
	// ultimate attack beam
	Ultimate ultimate;
	// hit flash effect shizzle
	// (should probably be moved into Sprite right?)
	bool effectActive = false;
	float effectTime = 0.0f;
	float effectDuration = 0.045f;
	// we don't wanna do boundary adjustment during our big entrance!
	bool isDoingBoundaryAdjustment = false;
	// this is the state that holds our logic
	Behavior* pBehavior;
	// rng engine for shia
	std::mt19937 rng = std::mt19937{ std::random_device{}() };
	// sounds
	const Sound* poop_sound = Codex<Sound>::Retrieve( L"Sounds\\fart2.wav" );
	const Sound* doit_sound1 = Codex<Sound>::Retrieve( L"Sounds\\just_doit1.mp3" );
	// timer for triggering behaviors
	FrameTimer timer;
};