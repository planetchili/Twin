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
	void BeginFrame();
	Color GetPixel( int x,int y ) const;
	void PutPixel( int x,int y,int r,int g,int b )
	{
		PutPixel( x,y,{ unsigned char( r ),unsigned char( g ),unsigned char( b ) } );
	}
	void PutPixel( int x,int y,Color c );
	void DrawSpriteNonChroma( int x,int y,const Surface& s );
	void DrawSpriteNonChroma( int x,int y,const RectI& srcRect,const Surface& s );
	void DrawSpriteNonChroma( int x,int y,RectI srcRect,const RectI& clip,const Surface& s );
	void DrawSprite( int x,int y,const Surface& s,Color chroma = Colors::Magenta );
	void DrawSprite( int x,int y,const RectI& srcRect,const Surface& s,Color chroma = Colors::Magenta );
	void DrawSprite( int x,int y,RectI srcRect,const RectI& clip,const Surface& s,Color chroma = Colors::Magenta );
	// this version of drawsprite substitutes all drawn pixel colors with the supplied color
	void DrawSpriteSubstitute( int x,int y,Color substitute,const Surface& s,Color chroma = Colors::Magenta );
	void DrawSpriteSubstitute( int x,int y,Color substitute,const RectI& srcRect,const Surface& s,Color chroma = Colors::Magenta );
	void DrawSpriteSubstitute( int x,int y,Color substitute,RectI srcRect,const RectI& clip,const Surface& s,Color chroma = Colors::Magenta );
	// this version of drawsprite has 50% transparency
	void DrawSpriteGhost( int x,int y,const Surface& s,Color chroma = Colors::Magenta );
	void DrawSpriteGhost( int x,int y,const RectI& srcRect,const Surface& s,Color chroma = Colors::Magenta );
	void DrawSpriteGhost( int x,int y,RectI srcRect,const RectI& clip,const Surface& s,Color chroma = Colors::Magenta );
	~Graphics();
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
	Color*                                              pSysBuffer = nullptr;
public:
	static constexpr int ScreenWidth = 800;
	static constexpr int ScreenHeight = 600;
	static RectI GetScreenRect();
};