#include "Bullet.h"
#include "Sound.h"

Bullet::Bullet( const Vec2 & pos,const Vec2 & dir )
	:
	Entity( pos,300.0f,8.0f,8.0f ),
	bullet_animation( 0,0,8,8,4,Codex<Surface>::Retrieve( L"Images\\fireball.bmp" ),0.1f )
{
	SetDirection( dir );
	// play fireball sound on fireball creation
	Codex<Sound>::Retrieve( L"Sounds\\fball.wav" )->Play( 0.75f,0.4f );
}

Bullet::~Bullet()
{
	// right now bullet only die if they hit something
	// so this is pretty sweet (prolly need to change l8r tho)
	Codex<Sound>::Retrieve( L"Sounds\\fhit.wav" )->Play( 0.9f,0.3f );;
}

void Bullet::ProcessLogic( const World & world )
{}

inline void Bullet::Draw( Graphics & gfx ) const
{
	// calculate drawing base
	const auto draw_pos = pos + draw_offset;
	// draw the bullet
	bullet_animation.Draw( (Vei2)draw_pos,gfx );
}

void Bullet::Update( World & world,float dt )
{
	pos += vel * dt;
	bullet_animation.Update( dt );
}

void Bullet::ApplyDamage( float )
{}