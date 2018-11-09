#pragma once

#include "Colors.h"
#include <string>
#include <vector>
#include "Rect.h"

class Surface
{
public:
	// loading filename that begins with "pm_"
	// will trigger alpha premultiply 'baking'
	Surface( const std::wstring& filename );
	Surface( int width,int height );
	void PutPixel( int x,int y,Color c );
	Color GetPixel( int x,int y ) const;
	int GetWidth() const;
	int GetHeight() const;
	RectI GetRect() const;
	void Fill( Color c );
	Color* Data();
	const Color* Data() const;
	int GetPitch() const;
	// this function performs alpha premultiplication
	// which enables more efficient alpha blending
	void BakeAlpha();
private:
	std::vector<Color> pixels;
	int width;
	int height;
};