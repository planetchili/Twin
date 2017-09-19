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

Game::Game( MainWindow& wnd )
	:
	wnd( wnd ),
	gfx( wnd )
{
	std::uniform_real_distribution<float> xd( 0,800 );
	std::uniform_real_distribution<float> yd( 0,600 );
	for( int n = 0; n < 12; n++ )
	{
		poos.emplace_back( Vec2{ xd( rng ),yd( rng ) } );
	}
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
	FrameTimer benchtimer;
#ifdef NDEBUG
	const auto dt = ft.Mark();
#else
	const auto dt = 1.0f / 60.0f;
#endif
	// process mouse messages while any remain
	while( !wnd.mouse.IsEmpty() )
	{
		const auto e = wnd.mouse.Read();
		// only interested in left mouse presses
		// fire in the hole!
		if( e.GetType() == Mouse::Event::Type::LPress )
		{
			// get direction of firing
			auto delta = (Vec2)e.GetPos() - chili.GetPos();
			// process delta to make it direction
			// if delta is 0 set to straight down
			if( delta == Vec2{ 0.0f,0.0f } )
			{
				delta = { 0.0f,1.0f };
			}
			// else normalize
			else
			{
				delta.Normalize();
			}
			// bullet spawn location constant
			const Vec2 bspawn = { 0.0f,-15.0f };
			// now spawn bullet!
			bullets.emplace_back( chili.GetPos() + bspawn,delta );
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


	// update all bullets
	for( auto& b : bullets )
	{
		b.Update( dt );
	}
	// my milkshake bring all the poos to the yard
	for( auto& poo : poos )
	{
		// poo movement logic
		{
			// flag for avoidance state
			bool avoiding = false;
			// if close to any enemy, avoid it
			for( const auto& other : poos )
			{
				// don't consider self
				if( &poo == &other )
				{
					continue;
				}
				// check if poo is within theshold (hardcoded here as thresh^2)
				const auto delta = poo.GetPos() - other.GetPos();
				const auto lensq = delta.GetLengthSq();
				if( lensq < 400.0f )
				{
					// avoiding state set
					avoiding = true;
					// case for poos at same location
					if( lensq == 0.0f )
					{
						poo.SetDirection( { -1.0f,1.0f } );
					}
					else
					{
						// normalize delta to get dir (reusing precalculated lensq)
						// if you would have just called Normalize() like a good boy...
						poo.SetDirection( delta / std::sqrt( lensq ) );
					}
					// no need to check other poos
					break;
				}
			}
			// check if in avoidance state, if so do not pursue
			if( !avoiding )
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
			}
		}
		poo.Update( dt );
		// calculate the poo hitbox once here
		const auto poo_hitbox = poo.GetHitbox();
		// chili take damage if collide with poo
		// a little redundancy here in generating same chili hitbox for each poo
		// but do we really care? (naw son)
		if( !chili.IsInvincible() && chili.GetHitbox().IsOverlappingWith( poo_hitbox ) )
		{
			chili.ApplyDamage();
			chili_hurt.Play( rng );
		}
		// check each bullet to see if coliding with current poo
		for( size_t i = 0u; i < bullets.size(); )
		{
			if( bullets[i].GetHitbox().IsOverlappingWith( poo_hitbox ) )
			{
				// remove bullet and activate poo hit effect
				remove_element( bullets,i );
				poo.ActivateEffect();
				// skip incrementing index (current index now holds the poo that was at end)
				continue;
			}
			// only increment i if bullet not removed
			i++;
		}
	}
	// clear all oob fballs
	{
		const auto screenrect = (RectF)gfx.GetScreenRect();
		for( size_t i = 0u; i < bullets.size(); )
		{
			if( !bullets[i].GetHitbox().IsOverlappingWith( screenrect ) )
			{
				// remove bullet if out of screen
				remove_element( bullets,i );
			}
			// only increment i if bullet not removed
			i++;
		}
	}
	// process benchmark
	OutputDebugStringA( (std::to_string( benchtimer.Mark() ) + '\n').c_str() );
}

void Game::ComposeFrame()
{
	font.DrawText( "becky.\nlemme smash.",{ 100,100 },Colors::White,gfx );

	// draw dem poos
	for( const auto& poo : poos )
	{
		poo.Draw( gfx );
		gfx.DrawRectThin( (RectI)poo.GetHitbox(),Colors::Red );
	}

	chili.Draw( gfx );
	gfx.DrawRectThin( (RectI)chili.GetHitbox(),Colors::Green );

	for( const auto& b : bullets )
	{
		b.Draw( gfx );
	}
}
