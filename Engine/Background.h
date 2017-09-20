#pragma once

#include "Graphics.h"
#include "Surface.h"
#include "SpriteEffect.h"

class Background
{
public:
	Background( const RectI& rect )
		:
		floor( "Images\\floor.bmp" ),
		origin( rect.TopLeft() )
	{
		// calculate grid dims so that there are enough tiles to fill screen
		gridWidth = div_int_ceil( rect.GetWidth(),floor.GetWidth() );
		gridHeight = div_int_ceil( rect.GetHeight(),floor.GetWidth() );
	}
	void Draw( Graphics& gfx ) const
	{
		const auto tileWidth = floor.GetWidth();
		const auto tileHeight = floor.GetHeight();
		for( int y = 0; y < gridHeight; y++ )
		{
			for( int x = 0; x < gridWidth; x++ )
			{
				gfx.DrawSprite( x * tileWidth + origin.x,y * tileHeight + origin.y,floor,
								SpriteEffect::Copy{} );
			}
		}
	}
private:
	// floor tile art
	Surface floor;
	// top left corner of background grid
	Vei2 origin;
	// grid dimensions in tiles
	int gridWidth;
	int gridHeight;
};