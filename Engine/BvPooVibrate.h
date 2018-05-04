#pragma once
#include "Poo.h"
#include <random>

class Poo::Vibrate : public Poo::Behavior
{
public:
	Behavior* Update( Poo& shia,class World& world,float dt ) override;
private:
	// how long the behavior lasts
	float duration = 1.5f;
	// controls rate of vibration
	float period = 0.16f;
	// amplitude of vibration speed
	float amplitude = 64.0f;
	// state timer
	float s_time = 0.0f;
};