#include "BvShiaBeamer.h"

Shia::Behavior* Shia::Beamer::Update( Shia& shia,World& world,float dt )
{
	//state_time += dt;
	//if( state_time >= 2.0f )
	//{
	//	return new Beamer;
	//}
	return nullptr;
}

void Shia::Beamer::Activate( Shia& shia,const World& world )
{
	Codex<Sound>::Retrieve( L"Sounds\\just_doit1.mp3" )->Play();
	shia.sprite.SetMode( SpriteMode::Beam );
	shia.ultimate.Activate();
}
