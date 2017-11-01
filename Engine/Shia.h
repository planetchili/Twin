#pragma once

#include "Vec2.h"
#include "Rect.h"
#include "Codex.h"
#include "Surface.h"
#include "Entity.h"

class Shia : public Entity
{
public:
	Shia( const Vec2& pos );
	// here the Shia does it's 'thinking' and decides its actions
	void ProcessLogic( const class World& world ) override;
	// here the Shia updates physical state based on the dt and the world
	void Update( World& world,float dt ) override;
	void ApplyDamage( float damage ) override;
	void Draw( Graphics& gfx ) const override;
private:
	const Surface* pShiaSurf = Codex<Surface>::Retrieve( L"Images\\pm_shia_test2.png" );
	// this value give the offset from the actual base of the
	// character to its drawing base
	Vec2 draw_offset = { -47.0f,-163.0f };
};