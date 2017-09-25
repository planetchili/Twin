#pragma once

#include "Graphics.h"
#include "Surface.h"
#include "SpriteEffect.h"
#include <vector>
#include <random>

class Background
{
public:
	Background( const RectI& bgRegion )
		:
		tileset( "Images\\floor5.bmp" ),
		origin( bgRegion.TopLeft() )
	{
		// calculate grid dims so that there are enough tiles to fill screen
		gridWidth = div_int_ceil( bgRegion.GetWidth(),tileSize );
		gridHeight = div_int_ceil( bgRegion.GetHeight(),tileSize );
		// generate tile rects
		for( int n = 0; n < nTiles; n++ )
		{
			tileRects.emplace_back(
				Vei2{ tileSize,0 } * n,tileSize,tileSize
			);
		}
		// reserve tile map space
		tiles.reserve( gridWidth * gridHeight );
		// populate map with random floor tiles
		std::mt19937 rng( std::random_device{}() );
		std::uniform_int_distribution<int> dist( 0,nTiles - 1 );
		for( int n = 0; n < gridWidth * gridHeight; n++ )
		{
			tiles.push_back( dist( rng ) );
		}
	}
	void Draw( Graphics& gfx ) const
	{
		for( int y = 0; y < gridHeight; y++ )
		{
			for( int x = 0; x < gridWidth; x++ )
			{
				gfx.DrawSprite( x * tileSize + origin.x,y * tileSize + origin.y,
								tileRects[GetTileAt( x,y )],
								tileset,SpriteEffect::Copy{}
				);
			}
		}
	}
private:
	int GetTileAt( int x,int y ) const
	{
		return tiles[y * gridWidth + x];
	}
private:
	// tileset image for background
	Surface tileset;
	// top left corner of background grid
	Vei2 origin;
	// vector of tile rects
	std::vector<RectI> tileRects;
	// grid of tiles (indices into the tileRect vector)
	std::vector<int> tiles;
	// number of tiles in set
	int nTiles = 6;
	// tile dimensions in pixels
	int tileSize = 32;
	// grid dimensions in tiles
	int gridWidth;
	int gridHeight;
};