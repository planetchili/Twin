#pragma once

#include "Vec2.h"
#include "Rect.h"
#include "Codex.h"
#include "Entity.h"
#include "SpriteElement.h"
#include "Sound.h"
#include <random>

class Shia : public Entity
{
	// the beefbrain is a state machine
	// here are it's states (states encapsulate behaviors)
	class BrainState
	{
	public:
		// make decisions and set controls of the lebeouf (the beef)
		virtual void ProcessLogic( Shia& shia,const class World& world ) const = 0;
		// transition / update state based on state of the world & shia
		// return new BrainState* on transition, otherwise nullptr
		virtual BrainState* Update( Shia& shia,class World& world,float dt ) = 0;
		// don't forget little vbro
		virtual ~BrainState() {}
		// this could be a thing (stack/queue of successor states)
		void SetSuccessorStates( std::vector<BrainState*> successors )
		{
			statePtrStack = std::move( successors );
		}
		// pass the torch
		BrainState* PassTorch()
		{
			// store the pointer to the next state
			auto ps = statePtrStack.back();
			statePtrStack.pop_back();
			ps->SetSuccessorStates( std::move( statePtrStack ) );
			return ps;
		}
		bool HasSuccessors() const
		{
			return !statePtrStack.empty();
		}
		// some bullshit to get around the fact of not enough info at construction
		virtual void Activate( Shia& shia,const class World& world ) {}
	private:
		std::vector<BrainState*> statePtrStack;
	};
	// move to target position
	class SlowRollState : public BrainState
	{
	public:
		SlowRollState( Shia& shia,const Vec2& target );
		void ProcessLogic( Shia& shia,const class World& world ) const override;
		BrainState* Update( Shia& shia,class World& world,float dt ) override;
	private:
		Vec2 target;
	};
	// just chill out where you be at
	class ChillState : public BrainState
	{
	public:
		ChillState( float duration = std::numeric_limits<float>::max() )
			:
			duration( duration )
		{}
		void ProcessLogic( Shia& shia,const class World& world ) const override
		{
			shia.SetDirection( { 0.0f,0.0f } );
		}
		BrainState* Update( Shia& shia,class World& world,float dt ) override;
		void Activate( Shia& shia,const class World& world ) override
		{
			shia.spriteIndex = 0;
		}
	private:
		float duration;
		float s_time = 0.0f;
	};
	// quadradit speedup slowdown towards target pos
	class EaseInto : public BrainState
	{
	public:
		EaseInto( const Vec2& target,float speed )
			:
			target( target ),
			spd( speed )
		{}
		void ProcessLogic( Shia& shia,const class World& world ) const override
		{
			const Vec2 toVector = target - shia.GetPos();
			const float dist = toVector.GetLength();
			shia.SetDirection( toVector / dist );
			shia.speed = k * (startDistance * dist - sq( dist ));
		}
		BrainState* Update( Shia& shia,class World& world,float dt ) override
		{
			if( (target - shia.GetPos()).GetLengthSq() < 4.0f )
			{
				if( HasSuccessors() )
				{
					return PassTorch();
				}
			}
			return nullptr;
		}
		virtual void Activate( Shia& shia,const class World& world ) override
		{
			startDistance = (target - shia.GetPos()).GetLength();
			k = 4.0f * spd / sq( startDistance );
		}
	private:
		Vec2 target;
		float spd;
		float startDistance;
		float k;
	};
	// charge at chili w/ impulse and drag
	// bounce off walls
	class Charge : public BrainState
	{
	public:
		Charge( float start_speed,float end_speed,float decel_k )
			:
			start_speed( start_speed ),
			end_speed_sq( sq( end_speed ) ),
			decel_k( decel_k )
		{}
		void ProcessLogic( Shia& shia,const class World& world ) const override
		{
			// is this function/phase even really needed bro?
		}
		BrainState* Update( Shia& shia,class World& world,float dt ) override
		{
			// ending condition is if speed goes slow enough
			if( shia.vel.GetLengthSq() < end_speed_sq )
			{
				if( HasSuccessors() )
				{
					return PassTorch();
				}
			}
			
			// constant deceleration independent of velocity
			shia.vel -= shia.vel.GetNormalized() * decel_k * dt;

			// maintain current state
			return nullptr;
		}
		virtual void Activate( Shia& shia,const class World& world ) override;
	private:
		float start_speed;
		// used for transition condition
		float end_speed_sq;
		// used to calculate drag/friction
		float decel_k;
	};
	// vibrate crazily
	// (used for pre-charge)
	class Wigout : public BrainState
	{
	public:
		Wigout( float duration,float period,float magnitude )
			:
			duration( duration ),
			period( period ),
			dist( 0.0f,magnitude )
		{}
		void ProcessLogic( Shia& shia,const class World& world ) const override
		{
			// is this function/phase even really needed bro?
		}
		BrainState* Update( Shia& shia,class World& world,float dt ) override
		{
			// update state duration timer
			s_time += dt;
			// ending condition is if time has elapsed
			if( s_time >= duration )
			{
				if( HasSuccessors() )
				{
					return PassTorch();
				}
			}

			// update vibration timer
			v_time += dt;
			if( v_time >= period )
			{
				shia.pos = base + Vec2{ dist( rng ),dist( rng ) };
				v_time = 0.0f;
			}
			
			// maintain current state
			return nullptr;
		}
		void Activate( Shia& shia,const class World& world ) override
		{
			base = shia.GetPos();
			shia.spriteIndex = 1;
			shia.GetCurrentSprite().Reset();
		}
	private:
		// how long the behavior lasts
		float duration;
		// controls rate of vibration
		float period;
		// vibration timer
		float v_time = 0.0f;
		// state timer
		float s_time = 0.0f;
		// base position
		Vec2 base;
		// random generation shiz for vibration
		std::normal_distribution<float> dist;
		std::mt19937 rng = std::mt19937( std::random_device{}() );
	};
	// shake-n poop
	// maybe have a random distribution of poops?
	class Poopin : public BrainState
	{
	public:
		Poopin( float duration,float period,float magnitude,int count,float poopin_shake_factor )
			:
			duration( duration ),
			period( period ),
			shake_dist( 0.0f,magnitude ),
			poopin_shake_factor( poopin_shake_factor )
		{
			// generate ascending sequence of poo times
			std::uniform_real_distribution<float> pd( 0.0f,duration );
			ptimes.reserve( count );
			for( int n = 0; n < count; n++ )
			{
				ptimes.push_back( pd( rng ) );
			}
			std::sort( ptimes.begin(),ptimes.end() );
			iNextPoop = ptimes.cbegin();
		}
		void ProcessLogic( Shia& shia,const class World& world ) const override
		{
			// is this function/phase even really needed bro?
		}
		BrainState* Update( Shia& shia,class World& world,float dt ) override;
		void Activate( Shia& shia,const class World& world ) override
		{
			base = shia.GetPos();
			shia.spriteIndex = 1;
		}
	private:
		// how long the behavior lasts
		float duration;
		// controls rate of vibration
		float period;
		// vibration timer
		float v_time = 0.0f;
		// shake factor for poopin
		float poopin_shake_factor;
		// state timer
		float s_time = 0.0f;
		// base position
		Vec2 base;
		// poop times
		std::vector<float> ptimes;
		// next scheduled poop
		std::vector<float>::const_iterator iNextPoop;
		// poop offset
		OffsetElement oe = OffsetElement( { 30.0f,-35.0f },{ -30.0f,-35.0f } );
		// poop sound
		const Sound* poop_sound = Codex<Sound>::Retrieve( L"Sounds\\fart2.wav" );
		// random generation shiz for vibration
		std::normal_distribution<float> shake_dist;
		std::normal_distribution<float> angle_dist = std::normal_distribution<float>( 0.0f,PI / 20.0f );
		std::normal_distribution<float> speed_dist = std::normal_distribution<float>( 500.0f,20.0f );
		std::mt19937 rng = std::mt19937( std::random_device{}() );
	};
	// a pass-through (instant) state
	// to make shia face chili
	class Faceoff : public BrainState
	{
	public:
		void ProcessLogic( Shia& shia,const class World& world ) const override
		{
			// is this function/phase even really needed bro?
		}
		BrainState* Update( Shia& shia,class World& world,float dt ) override
		{
			// immediately transition
			if( HasSuccessors() )
			{
				return PassTorch();
			}

			// maintain current state
			return nullptr;
		}
		virtual void Activate( Shia& shia,const class World& world ) override;
	};
public:
	Shia( const Vec2& pos );
	// we are violating rule of 3 (5) hard here bois
	// don't ever put shia in a box (container) :S
	~Shia();
	// actually, just delete that shit
	Shia( const Shia& ) = delete;
	Shia& operator=( const Shia& ) = delete;
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
	SpriteElement& GetCurrentSprite() const
	{
		return *spritePtrs[spriteIndex];
	}
private:
	// make this non-mutable after virtual effect driver is implemented!
	mutable std::vector<SpriteElement*> spritePtrs;
	// hit flash effect shizzle
	bool effectActive = false;
	float effectTime = 0.0f;
	float effectDuration = 0.045f;
	// this give the current sprite to be draw
	// should probably do some bullshit with enum for this stuff but meh
	int spriteIndex = 0;
	// we don't wanna do boundary adjustment during our big entrance!
	bool isDoingBoundaryAdjustment;
	// this is the state that holds our logic
	BrainState* pBrainState = new SlowRollState( *this,{ 368.0f,300.0f } );
};