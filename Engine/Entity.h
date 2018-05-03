#pragma once

#include "Vec2.h"
#include "Rect.h"
#include <vector>

class Entity
{
protected:
	// inherit from this class to create Entity behaviors
	template<class T>
	class Behavior
	{
	public:
		// don't forget little vbro
		virtual ~Behavior() {}
		// transition / update state based on state of the world & shia
		// return new BrainState* on transition, otherwise nullptr
		virtual Behavior* Update( T& shia,class World& world,float dt ) = 0;
		// some bullshit to get around the fact of not enough info at construction
		virtual void Activate( T& shia,const class World& world ) {}
		// this could be a thing (stack/queue of successor states)
		void SetSuccessorStates( std::vector<Behavior*> successors )
		{
			statePtrStack = std::move( successors );
		}
		// pass the torch
		Behavior* PassTorch()
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
	private:
		std::vector<Behavior*> statePtrStack;
	};
public:
	struct LessY
	{
	public:
		bool operator()( const Entity& lhs,const Entity& rhs ) const
		{
			return lhs.pos.y < rhs.pos.y;
		}
	};
public:
	Entity( const Vec2& pos,float speed,float hitwidth,float hitheight );
	virtual void Draw( class Graphics& gfx ) const = 0;
	// do entity logic, set movement, queue actions to be taken during update
	virtual void ProcessLogic( const class World& world ) = 0;
	// update physical state based on state of world, also affect world (spawn things)
	virtual void Update( class World& world,float dt ) = 0;
	// react to damage taken
	virtual void ApplyDamage( float damage ) = 0;
	const Vec2& GetPos() const;
	RectF GetHitbox() const;
	bool IsDead() const;
	bool IsReadyForRemoval() const;
	void DisplaceBy( const Vec2& d );
protected:
	// input dir is to be a normalized vector
	void SetDirection( const Vec2& dir );
	void SetDead();
	void SetReadyForRemoval();
	void SetSpeed( float speed );
protected:
	Vec2 pos;
	Vec2 vel = { 0.0f,0.0f };
	float hitbox_halfwidth;
	float hitbox_halfheight;
	float speed;
	// facing left is the default (all images will be left facing)
	// this flag is set when facing right (and thus mirroring is reqd)
	bool facingRight = false;
private:
	bool isDead = false;
	bool isReadyForRemoval = false;
};