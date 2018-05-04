#pragma once

#include "Codex.h"
#include "Sound.h"
#include "Surface.h"
#include "Entity.h"

class Poo : public Entity
{
private:
	enum class EffectState
	{
		Normal,
		Hit,
		Dying
	};
public:
	Poo( const Vec2& pos,const Vec2& vel = { 0.0f,0.0f } );
	void Draw( Graphics& gfx ) const override;
	// here the poo does it's 'thinking' and decides its actions
	void ProcessLogic( const class World& world ) override;
	// here the poo updates physical state based on the dt and the world
	void Update( World& world,float dt ) override;
	void ApplyDamage( float damage ) override;
	void DisplaceBy( const Vec2& d );

	// behaviors
	using Behavior = Entity::Behavior<Poo>;
	class Vibrate;
	class Charge;
	class Coast;

private:
	const Surface* pPooSurface = Codex<Surface>::Retrieve( L"Images\\poo.bmp" );
	// sound when poo dies
	const Sound* pDeathSound = Codex<Sound>::Retrieve( L"Sounds\\monster_death.wav" );
	// this value give the offset from the actual base of the
	// character to its drawing base
	Vec2 draw_offset = { -11.0f,-19.0f };
	static constexpr float dissolveDuration = 0.6f;
	static constexpr float hitFlashDuration = 0.045f;
	float effectTime = 0.0f;
	EffectState effectState = EffectState::Normal;
	// this is the state that holds our logic
	Behavior* pBehavior;
	// hitpoints
	int hp = 100;
};