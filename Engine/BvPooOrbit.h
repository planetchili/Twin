#pragma once
#include "Poo.h"
#include <random>

class Poo::Orbit : public Poo::Behavior
{
public:
	Orbit( float angle )
		:
		start_angle( angle )
	{}
	Behavior* Update( Poo& poo,class World& world,float dt ) override;
private:
	float radius = 75.0f;
	float orbit_ang_speed = PI * 0.5f;
	float speed = 240.0;
	float start_angle;
	// state timer (used to derive rotation angle)
	float s_time = 0.0f;
};