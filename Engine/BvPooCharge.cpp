#include "BvPooCharge.h"
#include "Poo.h"
#include "World.h"
#include "BvPooCoast.h"

Poo::Behavior* Poo::Charge::Update( Poo& poo,World& world,float dt )
{
	poo.speed = speed;
	poo.SetDirection( (world.GetChiliConst().GetPos() - poo.GetPos()).GetNormalized() );
	Codex<Sound>::Retrieve( L"sounds\\poo_charge.wav" )->Play();
	return PassTorch();
}
