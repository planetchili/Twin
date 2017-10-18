#pragma once

#include "Vec2.h"
#include "SpriteEffect.h"
#include "Codex.h"
#include "Sound.h"
#include "Surface.h"

class Poo
{
private:
	enum class EffectState
	{
		Normal,
		Hit,
		Dying
	};
public:
	Poo( const Vec2& pos );
	void Draw( Graphics& gfx ) const;
	// here the poo does it's 'thinking' and decides its actions
	void ProcessLogic( const class World& world );
	// here the poo updates physical state based on the dt and the world
	void Update( const World& world,float dt );
	void ApplyDamage( float damage );
	const Vec2& GetPos() const;
	RectF GetHitbox() const;
	bool IsDead() const;
	bool IsReadyForRemoval() const;
	void DisplaceBy( const Vec2& d );
private:
	// this does not perform normalization
	void SetDirection( const Vec2& dir );
private:
	const Surface* pPooSurface = Codex<Surface>::Retrieve( L"Images\\poo.bmp" );
	// sound when fireball hits poo
	const Sound* pHitSound = Codex<Sound>::Retrieve( L"Sounds\\fhit.wav" );
	// sound when poo dies
	const Sound* pDeathSound = Codex<Sound>::Retrieve( L"Sounds\\monster_death.wav" );
	Vec2 pos;
	// hitbox dimensions
	float hitbox_halfwidth = 11.0f;
	float hitbox_halfheight = 4.0f;
	// this value give the offset from the actual base of the
	// character to its drawing base
	Vec2 draw_offset = { -11.0f,-19.0f };
	Vec2 vel = { 0.0f,0.0f };
	// used to keep track of graphical facing (for sprite mirroring)
	float speed = 90.0f;
	static constexpr float dissolveDuration = 0.6f;
	static constexpr float hitFlashDuration = 0.045f;
	float effectTime = 0.0f;
	EffectState effectState = EffectState::Normal;
	bool isReadyForRemoval = false;
	// hitpoints
	int hp = 100;
};