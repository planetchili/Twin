/******************************************************************************************
*	Chili DirectX Framework Version 16.07.20											  *
*	Graphics.h																			  *
*	Copyright 2016 PlanetChili <http://www.planetchili.net>								  *
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
#include "ChiliWin.h"
#include <d3d11.h>
#include <wrl.h>
#include "ChiliException.h"
#include "Colors.h"
#include "Surface.h"
#include "Rect.h"
#include <cassert>
#include "BloomProcessor.h"

#define CHILI_GFX_EXCEPTION( hr,note ) Graphics::Exception( hr,note,_CRT_WIDE(__FILE__),__LINE__ )

class Graphics
{
public:
	class Exception : public ChiliException
	{
	public:
		Exception( HRESULT hr,const std::wstring& note,const wchar_t* file,unsigned int line );
		std::wstring GetErrorName() const;
		std::wstring GetErrorDescription() const;
		virtual std::wstring GetFullMessage() const override;
		virtual std::wstring GetExceptionType() const override;
	private:
		HRESULT hr;
	};
private:
	// vertex format for the framebuffer fullscreen textured quad
	struct FSQVertex
	{
		float x,y,z;		// position
		float u,v;			// texcoords
	};
public:
	Graphics( class HWNDKey& key );
	Graphics( const Graphics& ) = delete;
	Graphics& operator=( const Graphics& ) = delete;
	void EndFrame();
	void BeginFrame( Color bg = Colors::Black );
	Color GetPixel( int x,int y ) const;
	void PutPixel( int x,int y,int r,int g,int b )
	{
		PutPixel( x,y,{ unsigned char( r ),unsigned char( g ),unsigned char( b ) } );
	}
	void PutPixel( int x,int y,Color c );
	// draw a thin line rect [top-left:bottom-right)
	void DrawRectThin( const RectI& rect,Color color,const RectI& clip = GetScreenRect() )
	{
		// get clipped version of rectangle
		const auto clipped = rect.GetClippedTo( clip );
		// don't bother with degenerate rects
		// (this will also skip rects outside of clip)
		if( clipped.IsDegenerate() )
		{
			return;
		}
		// top line
		if( rect.top >= clip.top )
		{
			for( int x = clipped.left; x < clipped.right; x++ )
			{
				PutPixel( x,rect.top,color );
			}
		}
		// bottom line
		if( rect.bottom <= clip.bottom )
		{
			for( int x = clipped.left; x < clipped.right; x++ )
			{
				PutPixel( x,rect.bottom - 1,color );
			}
		}
		// left line
		if( rect.left >= clip.left )
		{
			for( int y = clipped.top; y < clipped.bottom; y++ )
			{
				PutPixel( rect.left,y,color );
			}
		}
		// right line
		if( rect.right <= clip.right )
		{
			for( int y = clipped.top; y < clipped.bottom; y++ )
			{
				PutPixel( rect.right - 1,y,color );
			}
		}
	}
	template<typename E>
	void DrawSprite( int x,int y,const Surface& s,E effect,bool reversed = false )
	{
		DrawSprite( x,y,s.GetRect(),s,effect,reversed );
	}
	template<typename E>
	void DrawSprite( int x,int y,const RectI& srcRect,const Surface& s,E effect,bool reversed = false )
	{
		DrawSprite( x,y,srcRect,GetScreenRect(),s,effect,reversed );
	}
	template<typename E>
	void DrawSprite( int x,int y,RectI srcRect,const RectI& clip,const Surface& s,E effect,bool reversed = false )
	{
		assert( srcRect.left >= 0 );
		assert( srcRect.right <= s.GetWidth() );
		assert( srcRect.top >= 0 );
		assert( srcRect.bottom <= s.GetHeight() );

		// mirror in x depending on reversed bool switch
		if( !reversed )
		{
			// cliping is different depending on mirroring status
			if( x < clip.left )
			{
				srcRect.left += clip.left - x;
				x = clip.left;
			}
			if( y < clip.top )
			{
				srcRect.top += clip.top - y;
				y = clip.top;
			}
			if( x + srcRect.GetWidth() > clip.right )
			{
				srcRect.right -= x + srcRect.GetWidth() - clip.right;
			}
			if( y + srcRect.GetHeight() > clip.bottom )
			{
				srcRect.bottom -= y + srcRect.GetHeight() - clip.bottom;
			}
			for( int sy = srcRect.top; sy < srcRect.bottom; sy++ )
			{
				for( int sx = srcRect.left; sx < srcRect.right; sx++ )
				{
					effect(
						// no mirroring
						s.GetPixel( sx,sy ),
						x + sx - srcRect.left,
						y + sy - srcRect.top,
						*this
					);
				}
			}
		}
		else
		{
			if( x < clip.left )
			{
				srcRect.right -= clip.left - x;
				x = clip.left;
			}
			if( y < clip.top )
			{
				srcRect.top += clip.top - y;
				y = clip.top;
			}
			if( x + srcRect.GetWidth() > clip.right )
			{
				srcRect.left += x + srcRect.GetWidth() - clip.right;
			}
			if( y + srcRect.GetHeight() > clip.bottom )
			{
				srcRect.bottom -= y + srcRect.GetHeight() - clip.bottom;
			}
			const int xOffset = srcRect.left + srcRect.right - 1;
			for( int sy = srcRect.top; sy < srcRect.bottom; sy++ )
			{
				for( int sx = srcRect.left; sx < srcRect.right; sx++ )
				{
					effect(
						// mirror in x
						s.GetPixel( xOffset - sx,sy ),
						x + sx - srcRect.left,
						y + sy - srcRect.top,
						*this
					);
				}
			}
		}
	}
	template<typename E>
	void DrawTriangle( const Vec2& v0,const Vec2& v1,const Vec2& v2,const RectI& clip,E effect )
	{
		// using pointers so we can swap (for sorting purposes)
		const Vec2* pv0 = &v0;
		const Vec2* pv1 = &v1;
		const Vec2* pv2 = &v2;

		// sorting vertices by y
		if( pv1->y < pv0->y ) std::swap( pv0,pv1 );
		if( pv2->y < pv1->y ) std::swap( pv1,pv2 );
		if( pv1->y < pv0->y ) std::swap( pv0,pv1 );

		if( pv0->y == pv1->y ) // natural flat top
		{
			// sorting top vertices by x
			if( pv1->x < pv0->x ) std::swap( pv0,pv1 );
			DrawFlatTopTriangle( *pv0,*pv1,*pv2,clip,effect );
		}
		else if( pv1->y == pv2->y ) // natural flat bottom
		{
			// sorting bottom vertices by x
			if( pv2->x < pv1->x ) std::swap( pv1,pv2 );
			DrawFlatBottomTriangle( *pv0,*pv1,*pv2,clip,effect );
		}
		else // general triangle
		{
			// find splitting vertex
			const float alphaSplit =
				(pv1->y - pv0->y) /
				(pv2->y - pv0->y);
			const Vec2 vi = *pv0 + (*pv2 - *pv0) * alphaSplit;

			if( pv1->x < vi.x ) // major right
			{
				DrawFlatBottomTriangle( *pv0,*pv1,vi,clip,effect );
				DrawFlatTopTriangle( *pv1,vi,*pv2,clip,effect );
			}
			else // major left
			{
				DrawFlatBottomTriangle( *pv0,vi,*pv1,clip,effect );
				DrawFlatTopTriangle( vi,*pv1,*pv2,clip,effect );
			}
		}
	}
	~Graphics();
private:
	// workhorse triangle drawing functions
	template<typename E>
	void DrawFlatTopTriangle( const Vec2& v0,const Vec2& v1,const Vec2& v2,const RectI& clip,E effect )
	{
		// calulcate slopes in screen space
		float m0 = (v2.x - v0.x) / (v2.y - v0.y);
		float m1 = (v2.x - v1.x) / (v2.y - v1.y);

		// calculate start and end scanlines
		const int yStart = std::max( clip.top,(int)ceil( v0.y - 0.5f ) );
		const int yEnd = std::min( clip.bottom,(int)ceil( v2.y - 0.5f ) ); // the scanline AFTER the last line drawn

		for( int y = yStart; y < yEnd; y++ )
		{
			// caluclate start and end points (x-coords)
			// add 0.5 to y value because we're calculating based on pixel CENTERS
			const float px0 = m0 * (float( y ) + 0.5f - v0.y) + v0.x;
			const float px1 = m1 * (float( y ) + 0.5f - v1.y) + v1.x;

			// calculate start and end pixels
			const int xStart = std::max( clip.left,(int)ceil( px0 - 0.5f ) );
			const int xEnd = std::min( clip.right,(int)ceil( px1 - 0.5f ) ); // the pixel AFTER the last pixel drawn

			for( int x = xStart; x < xEnd; x++ )
			{
				effect( x,y,*this );
			}
		}
	}
	template<typename E>
	void DrawFlatBottomTriangle( const Vec2& v0,const Vec2& v1,const Vec2& v2,const RectI& clip,E effect )
	{
		// calulcate slopes in screen space
		float m0 = (v1.x - v0.x) / (v1.y - v0.y);
		float m1 = (v2.x - v0.x) / (v2.y - v0.y);

		// calculate start and end scanlines
		const int yStart = std::max( clip.top,(int)ceil( v0.y - 0.5f ) );
		const int yEnd = std::min( clip.bottom,(int)ceil( v2.y - 0.5f ) ); // the scanline AFTER the last line drawn

		for( int y = yStart; y < yEnd; y++ )
		{
			// caluclate start and end points
			// add 0.5 to y value because we're calculating based on pixel CENTERS
			const float px0 = m0 * (float( y ) + 0.5f - v0.y) + v0.x;
			const float px1 = m1 * (float( y ) + 0.5f - v0.y) + v0.x;

			// calculate start and end pixels
			const int xStart = std::max( clip.left,(int)ceil( px0 - 0.5f ) );
			const int xEnd = std::min( clip.right,(int)ceil( px1 - 0.5f ) ); // the pixel AFTER the last pixel drawn

			for( int x = xStart; x < xEnd; x++ )
			{
				effect( x,y,*this );
			}
		}
	}
private:
	Microsoft::WRL::ComPtr<IDXGISwapChain>				pSwapChain;
	Microsoft::WRL::ComPtr<ID3D11Device>				pDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>			pImmediateContext;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>		pRenderTargetView;
	Microsoft::WRL::ComPtr<ID3D11Texture2D>				pSysBufferTexture;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	pSysBufferTextureView;
	Microsoft::WRL::ComPtr<ID3D11PixelShader>			pPixelShader;
	Microsoft::WRL::ComPtr<ID3D11VertexShader>			pVertexShader;
	Microsoft::WRL::ComPtr<ID3D11Buffer>				pVertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11InputLayout>			pInputLayout;
	Microsoft::WRL::ComPtr<ID3D11SamplerState>			pSamplerState;
	D3D11_MAPPED_SUBRESOURCE							mappedSysBufferTexture;
	Surface                                             sysBuffer;
	BloomProcessor										bp = { sysBuffer };
public:
	static constexpr int ScreenWidth = 800;
	static constexpr int ScreenHeight = 600;
	static RectI GetScreenRect();
};

#ifndef GOD_GRAPHICS
#include "SpriteEffect.h"
extern template
void Graphics::DrawSprite<SpriteEffect::Copy>( int x,int y,RectI srcRect,const RectI& clip,const Surface& s,SpriteEffect::Copy effect,bool reversed );
extern template
void Graphics::DrawSprite<SpriteEffect::Chroma>( int x,int y,RectI srcRect,const RectI& clip,const Surface& s,SpriteEffect::Chroma effect,bool reversed );
extern template
void Graphics::DrawSprite<SpriteEffect::Substitution>( int x,int y,RectI srcRect,const RectI& clip,const Surface& s,SpriteEffect::Substitution effect,bool reversed );
extern template
void Graphics::DrawSprite<SpriteEffect::Ghost>( int x,int y,RectI srcRect,const RectI& clip,const Surface& s,SpriteEffect::Ghost effect,bool reversed );
extern template
void Graphics::DrawSprite<SpriteEffect::DissolveHalfTint>( int x,int y,RectI srcRect,const RectI& clip,const Surface& s,SpriteEffect::DissolveHalfTint effect,bool reversed );
extern template
void Graphics::DrawSprite<SpriteEffect::AlphaBakedSubstitution::Functor>( int x,int y,RectI srcRect,const RectI& clip,const Surface& s,SpriteEffect::AlphaBakedSubstitution::Functor effect,bool reversed );
extern template
void Graphics::DrawSprite<SpriteEffect::AlphaBlendBaked::Functor>( int x,int y,RectI srcRect,const RectI& clip,const Surface& s,SpriteEffect::AlphaBlendBaked::Functor effect,bool reversed );
#endif
