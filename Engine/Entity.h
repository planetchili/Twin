#pragma once

#include "Vec2.h"
#include "Rect.h"

class Entity
{
public:
	Entity( const Vec2& pos,float speed,float hitwidth,float hitheight );
	virtual void Draw( class Graphics& gfx ) const = 0;
	// do entity logic, set movement, queue actions to be taken during update
	virtual void ProcessLogic( const class World& world ) = 0;
	// update physical state based on state of world, also affect world (spawn things)
	virtual void Update( const World& world,float dt ) = 0;
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
protected:
	Vec2 pos;
	Vec2 vel = { 0.0f,0.0f };
	float hitbox_halfwidth;
	float hitbox_halfheight;
	float speed;
	bool facingLeft = false;
private:
	bool isDead = false;
	bool isReadyForRemoval = false;
};