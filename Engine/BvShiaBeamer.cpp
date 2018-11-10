#include "BvShiaBeamer.h"
#include "World.h"

Shia::Behavior* Shia::Beamer::Update( Shia& shia,World& world,float dt )
{
	state_time += dt;
	if( shia.ultimate.IsBeaming() && !beaming )
	{
		Graphics::cam.TurnOn();
		beam.Play( 1.0f,0.6f );
		beaming = true;
	}
	return nullptr;
}

void Shia::Beamer::Activate( Shia& shia,const World& world )
{
	Codex<Sound>::Retrieve( L"Sounds\\just_doit1.mp3" )->Play();
	shia.sprite.SetMode( SpriteMode::Beam );
	shia.ultimate.Activate();
}
