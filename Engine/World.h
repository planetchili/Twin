#pragma once

#include "Chili.h"
#include "Poo.h"
#include "Bullet.h"
#include "Background.h"
#include "Boundary.h"
#include "Sound.h"
#include "Shia.h"
#include "Keyboard.h"
#include "Mouse.h"
#include <random>
#include <vector>

class World
{
public:
	World( const RectI& screenRect,Keyboard& kbd,Mouse& mouse );
	void ProcessLogic();
	void Update( float dt );
	void Draw( Graphics& gfx ) const;
	void SpawnBullet( Bullet bullet );
	void SpawnPoo( const Vec2& pos,const Vec2& vel );
	const std::vector<Poo>& GetPoosConst() const;
	const Chili& GetChiliConst() const;
	const std::vector<Bullet>& GetBulletsConst() const;
	const Boundary& GetBoundsConst() const;
private:
	std::mt19937 rng = std::mt19937( std::random_device{}() );
	Sound bgm = Sound( L"Sounds\\danger.mp3",Sound::LoopType::AutoFullSound );
	// scenery underlayer
	Background bg1;
	// scenery overlayer
	Background bg2;
	Chili chili;
	Shia shia;
	std::vector<Poo> poos;
	std::vector<Bullet> bullets;
	// boundary that characters must remain inside of
	Boundary bounds = RectF{ 32.0f,768.0f,96.0f,576.0f + 64.0f };
};