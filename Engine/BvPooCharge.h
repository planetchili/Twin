#pragma once
#include "Poo.h"
#include <random>

class Poo::Charge : public Poo::Behavior
{
public:
	Behavior* Update( Poo& poo,class World& world,float dt ) override;
private:
	float speed = 800.0f;
};