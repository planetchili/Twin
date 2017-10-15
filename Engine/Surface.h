#pragma once

#include "Colors.h"
#include <string>
#include "Rect.h"

class Surface
{
public:
	Surface( const std::wstring& filename );
	Surface( int width,int height );
	Surface( const Surface& );
	~Surface();
	Surface& operator=( const Surface& );
	void PutPixel( int x,int y,Color c );
	Color GetPixel( int x,int y ) const;
	int GetWidth() const;
	int GetHeight() const;
	RectI GetRect() const;
	// this function performs alpha premultiplication
	// which enables more efficient alpha blending
	void BakeAlpha();
private:
	Color* pPixels = nullptr;
	int width;
	int height;
};