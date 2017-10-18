#pragma once

#include "Chili.h"
#include "Poo.h"
#include "Bullet.h"
#include "Background.h"
#include "Boundary.h"
#include "Sound.h"
#include "Keyboard.h"
#include "Mouse.h"
#include <random>
#include <vector>

class World
{
public:
	World( const RectI& screenRect );
	void HandleInput( Keyboard& kbd,Mouse& mouse );
	void Update( float dt );
	void Draw( Graphics& gfx ) const;
	void SpawnBullet( Bullet bullet );
private:
	std::mt19937 rng = std::mt19937( std::random_device{}() );
	Sound bgm = Sound( L"Sounds\\come.mp3",Sound::LoopType::AutoFullSound );
	// scenery underlayer
	Background bg1;
	// scenery overlayer
	Background bg2;
	Chili chili = Vec2{ 300.0f,300.0f };
	std::vector<Poo> poos;
	std::vector<Bullet> bullets;
	// boundary that characters must remain inside of
	Boundary bounds = RectF{ 32.0f,768.0f,96.0f,576.0f + 64.0f };
};