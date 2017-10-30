#pragma once

#include "Vec2.h"
#include "Rect.h"
#include "Codex.h"
#include "Surface.h"
#include "Graphics.h"

class Shia
{
public:
	Shia( const Vec2& pos );
	// here the Shia does it's 'thinking' and decides its actions
	void ProcessLogic( const class World& world );
	// here the Shia updates physical state based on the dt and the world
	void Update( const World& world,float dt );
	void ApplyDamage( float damage );
	const Vec2& GetPos() const;
	RectF GetHitbox() const;
	bool IsDead() const;
	bool IsReadyForRemoval() const;
	void DisplaceBy( const Vec2& d );
	void Draw( Graphics& gfx ) const;
	void SetDirection( const Vec2& dir );
private:
	const Surface* pShiaSurf = Codex<Surface>::Retrieve( L"Images\\pm_shia_test1.png" );
	Vec2 pos;
	Vec2 vel = { 0.0f,0.0f };
	float speed = 110.0f;
	// hitbox dimensions
	float hitbox_halfwidth = 10.0f;
	float hitbox_halfheight = 9.0f;
	// this value give the offset from the actual base of the
	// character to its drawing base
	Vec2 draw_offset = { -60.0f,-180.0f };
	// used to keep track of graphical facing (for sprite mirroring)
	bool facingRight = true;
};