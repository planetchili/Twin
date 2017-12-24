#pragma once
#include "Shia.h"
#include "Vec2.h"

class Shia::Decide : public Shia::Behavior
{
public:
	Decide( std::mt19937& rng );
	Behavior* Update( Shia& shia,class World& world,float dt ) override;
	void SetupCharge();
	void SetupChase();
	void SetupUltimate();
private:
	Vec2 waypoints[5] = {
		{ 150.0f,150.0f },
		{ 650.0f,150.0f },
		{ 150.0f,450.0f },
		{ 650.0f,450.0f },
		{ 368.0f,300.0f } // center
	};
	std::mt19937& rng;
};