/****************************************************************************************** 
 *	Chili DirectX Framework Version 16.07.20											  *	
 *	Game.h																				  *
 *	Copyright 2016 PlanetChili.net <http://www.planetchili.net>							  *
 *																						  *
 *	This file is part of The Chili DirectX Framework.									  *
 *																						  *
 *	The Chili DirectX Framework is free software: you can redistribute it and/or modify	  *
 *	it under the terms of the GNU General Public License as published by				  *
 *	the Free Software Foundation, either version 3 of the License, or					  *
 *	(at your option) any later version.													  *
 *																						  *
 *	The Chili DirectX Framework is distributed in the hope that it will be useful,		  *
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of						  *
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the						  *
 *	GNU General Public License for more details.										  *
 *																						  *
 *	You should have received a copy of the GNU General Public License					  *
 *	along with The Chili DirectX Framework.  If not, see <http://www.gnu.org/licenses/>.  *
 ******************************************************************************************/
#pragma once

#include "Keyboard.h"
#include "Mouse.h"
#include "Graphics.h"
#include "Surface.h"
#include "Animation.h"
#include "FrameTimer.h"
#include "Chili.h"
#include "Font.h"
#include "SoundEffect.h"
#include "Poo.h"
#include <random>
#include "Bullet.h"
#include "Background.h"

class Game
{
public:
	Game( class MainWindow& wnd );
	Game( const Game& ) = delete;
	Game& operator=( const Game& ) = delete;
	void Go();
private:
	void ComposeFrame();
	void UpdateModel();
	/********************************/
	/*  User Functions              */
	/********************************/
private:
	MainWindow& wnd;
	Graphics gfx;
	/********************************/
	/*  User Variables              */
	FrameTimer ft;
	std::mt19937 rng = std::mt19937( std::random_device{}() );
	Font font = "Images\\Fixedsys16x28.bmp";
	// sound matrix when chili is hit
	SoundEffect chili_hurt = SoundEffect(
		{ L"Sounds\\Isaac_Hurt_Grunt0.mp3",
		  L"Sounds\\Isaac_Hurt_Grunt1.mp3",
		  L"Sounds\\Isaac_Hurt_Grunt2.mp3" },
		  true,0.03f );
	// sound when fireball hits enemy
	Sound fhit = L"Sounds\\fhit.wav";
	// sound when fireball is shot
	Sound fball = L"Sounds\\fball.wav";
	// sound when enemy dies
	Sound death = L"Sounds\\monster_death.wav";
	Sound bgm = Sound( L"Sounds\\come.mp3",Sound::LoopType::AutoFullSound );
	// scenery underlayer
	Background bg1;
	// scenery overlayer
	Background bg2;
	Chili chili = Vec2{ 300.0f,300.0f };
	std::vector<Poo> poos;
	std::vector<Bullet> bullets;
	/********************************/
};