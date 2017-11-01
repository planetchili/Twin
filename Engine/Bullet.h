#pragma once

#include "Vec2.h"
#include "Animation.h"
#include "Rect.h"
#include "Codex.h"
#include "Entity.h"

class Bullet : public Entity
{
public:
	Bullet( const Vec2& pos,const Vec2& dir );
	// we will have to make virtual dtor in Entity at some point
	// but not today ;)
	~Bullet();
	void ProcessLogic( const class World& world ) override;
	void Draw( class Graphics& gfx ) const override;
	void Update( class World& world,float dt ) override;
	void ApplyDamage( float ) override;
private:
	Animation bullet_animation;
	// this value give the offset from the actual base of the
	// character to its drawing base
	Vec2 draw_offset = { -4.0f,-4.0f };
};