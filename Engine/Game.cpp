/****************************************************************************************** 
 *	Chili DirectX Framework Version 16.07.20											  *	
 *	Game.cpp																			  *
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
#include "MainWindow.h"
#include "Game.h"

Game::Game( MainWindow& wnd )
	:
	wnd( wnd ),
	gfx( wnd )
{
	poos.emplace_back( Vec2{ 10,10 } );
	poos.emplace_back( Vec2{ 700,10 } );
	poos.emplace_back( Vec2{ 600,500 } );
	poos.emplace_back( Vec2{ 10,500 } );
}

void Game::Go()
{
	gfx.BeginFrame( { 64,64,64 } );
	UpdateModel();
	ComposeFrame();
	gfx.EndFrame();
}

void Game::UpdateModel()
{
	const auto dt = ft.Mark();
	// process key messages while any remain
	while( !wnd.kbd.KeyIsEmpty() )
	{
		const auto e = wnd.kbd.ReadKey();
		// only interested in space bar presses
		if( e.IsPress() && e.GetCode() == VK_SPACE )
		{
			chili.ActivateEffect();
			for( auto& poo : poos )
			{
				poo.ActivateEffect();
			}
			hit.Play();
		}
	}
	// process arrow keys state to set direction
	Vec2 dir = { 0.0f,0.0f };
	if( wnd.kbd.KeyIsPressed( VK_UP ) )
	{
		dir.y -= 1.0f;
	}
	if( wnd.kbd.KeyIsPressed( VK_DOWN ) )
	{
		dir.y += 1.0f;
	}
	if( wnd.kbd.KeyIsPressed( VK_LEFT ) )
	{
		dir.x -= 1.0f;
	}
	if( wnd.kbd.KeyIsPressed( VK_RIGHT ) )
	{
		dir.x += 1.0f;
	}
	chili.SetDirection( dir );
	// update character
	chili.Update( dt );

	// my milkshake bring all the poos to the yard
	for( auto& poo : poos )
	{
		const auto delta = chili.GetPos() - poo.GetPos();
		// we only wanna move if not already really close to target pos
		// (prevents vibrating around target point; 3.0 just a number pulled out of butt)
		if( delta.GetLengthSq() > 3.0f )
		{
			poo.SetDirection( delta.GetNormalized() );
		}
		else
		{
			poo.SetDirection( { 0.0f,0.0f } );
		}
		poo.Update( dt );
	}
}

void Game::ComposeFrame()
{
	font.DrawText( "becky.\nlemme smash.",{ 100,100 },Colors::White,gfx );

	// draw dem poos
	for( const auto& poo : poos )
	{
		poo.Draw( gfx );
	}

	chili.Draw( gfx );
	// for eyeballing the proper draw offset in Chili
	gfx.PutPixel( 300,300,Colors::Yellow );
}
