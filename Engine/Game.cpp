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
#include "ChiliUtil.h"
#include <algorithm>
#include <functional>


Game::Game( MainWindow& wnd )
	:
	wnd( wnd ),
	gfx( wnd ),
	world( gfx.GetScreenRect(),wnd.kbd,wnd.mouse )
{}

void Game::Go()
{
	UpdateModel();
	ComposeFrame();
	gfx.EndFrame();
}

void Game::UpdateModel()
{
#ifdef NDEBUG
	const auto dt = ft.Mark();
#else
	const auto dt = 1.0f / 60.0f;
#endif
	world.ProcessLogic();
	world.Update( dt );
}

void Game::ComposeFrame()
{
	world.Draw( gfx );
	gfx.DrawTriangle( { 100.0f,100.0f },{ 700.0f,200.0f },{ 400.0f,500.0f },
		[src_pre = Color{ Colors::White.GetR() / 2u,Colors::White.GetG() / 2u,Colors::White.GetB() / 2u }]
		( int x,int y,Graphics& gfx )
		{
			const Color dst = gfx.GetPixel( x,y );
			const int rb = (((dst.dword & 0xFF00FFu) * 128u) >> 8) & 0xFF00FFu;
			const int g = (((dst.dword & 0x00FF00u) * 128u) >> 8) & 0x00FF00u;
			gfx.PutPixel( x,y,rb + g + src_pre.dword );
		} 
	);
}
