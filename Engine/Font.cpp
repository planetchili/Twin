#include "Font.h"
#include <cassert>

Font::Font( const std::string& filename,Color chroma )
	:
	surface( filename ),
	// calculate glyph dimensions from bitmap dimensions
	glyphWidth( surface.GetWidth() / nColumns ),
	glyphHeight( surface.GetHeight() / nRows ),
	chroma( chroma )
{
	// verify that bitmap had valid dimensions
	assert( glyphWidth * nColumns == surface.GetWidth() );
	assert( glyphHeight * nRows == surface.GetHeight() );
}

void Font::DrawText( const std::string& text,const Vei2& pos,Graphics& gfx ) const
{
	// curPos is the pos that we are drawing to on the screen
	auto curPos = pos;
	for( auto c : text )
	{
		// only draw characters that are on the font sheet
		// firstChar + 1 because might as well skip ' ' as well
		if( c >= firstChar + 1 && c <= lastChar )
		{
			gfx.DrawSprite( curPos.x,curPos.y,MapGlyphRect( c ),surface,chroma );
		}
		// advance screen pos for next character
		curPos.x += glyphWidth;
	}
}

RectI Font::MapGlyphRect( char c ) const
{
	assert( c >= firstChar && c <= lastChar );
	// font sheet glyphs start at ' ', calculate index into sheet
	const int glyphIndex = c - ' ';
	// map 1d glyphIndex to 2D coordinates
	const int yGlyph = glyphIndex / nColumns;
	const int xGlyph = glyphIndex % nColumns;
	// convert the sheet grid coords to pixel coords in sheet
	return RectI(
		{ xGlyph * glyphWidth,yGlyph * glyphHeight },
		glyphWidth,glyphHeight
	);
}
