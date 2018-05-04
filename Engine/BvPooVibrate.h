#pragma once
#include "Poo.h"
#include <random>

class Poo::Vibrate : public Poo::Behavior
{
public:
	Behavior* Update( Poo& shia,class World& world,float dt ) override;
private:
	// how long the behavior lasts
	float duration = 0.8f;
	// controls rate of vibration
	float period = 6.0f;
	// amplitude of vibration speed
	float amplitude = 2.0f;
	// state timer
	float s_time = 0.0f;
};