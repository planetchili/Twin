#pragma once

#include "Vec2.h"
#include "Rect.h"
#include "Codex.h"
#include "Surface.h"
#include "Entity.h"

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
		virtual BrainState* Update( Shia& shia,const class World& world,float dt ) = 0;
		// don't forget little vbro
		virtual ~BrainState() {}
	};
	// move to target position
	class SlowRollState : public BrainState
	{
	public:
		SlowRollState( Shia& shia,const Vec2& target );
		void ProcessLogic( Shia& shia,const class World& world ) const override;
		BrainState* Update( Shia& shia,const class World& world,float dt ) override;
	private:
		Vec2 target;
	};
	// just chill out where you be at
	class ChillState : public BrainState
	{
	public:
		void ProcessLogic( Shia& shia,const class World& world ) const override
		{
			shia.SetDirection( { 0.0f,0.0f } );
		}
		BrainState* Update( Shia& shia,const class World& world,float dt ) override
		{
			return nullptr;
		}
	};
	class EaseInto : public BrainState
	{
	public:
		EaseInto( Shia& shia,const Vec2& target,float speed )
			:
			target( target ),
			startDistance( (target - shia.GetPos()).GetLength() ),
			k( 4.0f * speed / sq( startDistance ) )
		{}
		void ProcessLogic( Shia& shia,const class World& world ) const override
		{
			const Vec2 toVector = target - shia.GetPos();
			const float dist = toVector.GetLength();
			shia.SetDirection( toVector / dist );
			shia.speed = k * (startDistance * dist - sq( dist ));
		}
		BrainState* Update( Shia& shia,const class World& world,float dt ) override
		{
			if( (target - shia.GetPos()).GetLengthSq() < 4.0f )
			{
				return new ChillState;
			}
			return nullptr;
		}
	private:
		Vec2 target;
		float startDistance;
		float k;
	};
public:
	Shia( const Vec2& pos );
	// here the Shia does it's 'thinking' and decides its actions
	void ProcessLogic( const class World& world ) override;
	// here the Shia updates physical state based on the dt and the world
	void Update( class World& world,float dt ) override;
	void ApplyDamage( float damage ) override;
	void Draw( Graphics& gfx ) const override;
private:
	const Surface* pShiaSurf = Codex<Surface>::Retrieve( L"Images\\pm_shia_test2.png" );
	// this value give the offset from the actual base of the
	// character to its drawing base
	Vec2 draw_offset = { -47.0f,-163.0f };
	// hit flash effect shizzle
	bool effectActive = false;
	float effectTime = 0.0f;
	float effectDuration = 0.045f;
	// we don't wanna do boundary adjustment during our big entrance!
	bool isDoingBoundaryAdjustment;
	// this is the state that holds our logic
	BrainState* pBrainState = new SlowRollState( *this,{ 368.0f,300.0f } );
};