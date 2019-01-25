#pragma once

#include "Codex.h"
#include "Sound.h"
#include "Surface.h"
#include "Entity.h"

class Door : public Entity
{
public:
	Door( const Vec2& pos );
	void Draw( Graphics& gfx ) const override;
	// here the poo does it's 'thinking' and decides its actions
	void ProcessLogic( const class World& world ) override {}
	// here the poo updates physical state based on the dt and the world
	void Update( World& world,float dt ) override;
	void ApplyDamage( float damage ) override {}

	// behaviors
	using Behavior = Entity::Behavior<Door>;
	class Vibrate;
	class Charge;

private:
	const Surface* pLeft = Codex<Surface>::Retrieve( L"Images\\pm_door-l.png" );
	const Surface* pRight = Codex<Surface>::Retrieve( L"Images\\pm_door-r.png" );
	float halfSeparation = 0.0f;
	// door open sound?
	//const Sound* pDeathSound = Codex<Sound>::Retrieve( L"Sounds\\monster_death.wav" );
};