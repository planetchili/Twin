#pragma once

#include "Vec2.h"
#include "Rect.h"
#include <vector>
#include <algorithm>
#include <iterator>
#ifndef NDEBUG
#include <sstream>
#include "TypeName.h"
#include "ChiliWin.h"
#endif

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
		// transition / update state based on state of the world & entity
		// return new Behavior* on transition, otherwise nullptr
		virtual Behavior* Update( T& entity,class World& world,float dt ) = 0;
		// allows us to run deferred init at activation instead of behavior construction
		virtual void Activate( T& entity,const class World& world ) {}
		// set successor state vector (stack)
		void SetSuccessorStates( std::vector<Behavior*> successors )
		{
			statePtrStack = std::move( successors );
		}
		// push one successor into state vector
		void PushSuccessorState( Behavior* successor )
		{
			statePtrStack.push_back( successor );
		}
		// push multiple successors into state vector
		void PushSuccessorStates( std::vector<Behavior*> successors )
		{
			std::copy(
				successors.begin(),successors.end(),
				std::back_inserter( statePtrStack )
			);
		}
		// transition to next behavior in stack
		Behavior* PassTorch()
		{
#ifndef NDEBUG
			std::wostringstream oss;
			oss << L"$BC [" << type_name_wide( *this ) << "] >> ["
				<< type_name_wide( *statePtrStack.back() ) << "]" << std::endl;
			OutputDebugString( oss.str().c_str() );
#endif
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
	const Vec2& GetVel() const;
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