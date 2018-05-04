#pragma once
#include "Poo.h"
#include <random>

class Poo::Coast : public Poo::Behavior
{
public:
	Behavior* Update( Poo& poo,class World& world,float dt ) override;
private:
	float decel_per_sec = 60.0f;
};