#pragma once

#include "Colors.h"
#include "Graphics.h"

// NOTE: to enable debug optimization of effect, add to dummy() in GraphicsOD.cpp

namespace SpriteEffect
{
	// all virtual effect drivers inherit from this
	// provides all interface drawing functions
	class Driver
	{
	public:
		virtual void DrawSprite( Graphics& gfx,const Vei2& pos,const RectI& srcRect,const RectI& clip,const Surface& s,bool reversed = false ) const = 0;
	};
	class Chroma
	{
	public:
		Chroma( Color c )
			:
			chroma( c )
		{}
		void operator()( Color cSrc,int xDest,int yDest,Graphics& gfx ) const
		{
			if( cSrc != chroma )
			{
				gfx.PutPixel( xDest,yDest,cSrc );
			}
		}
	private:
		Color chroma;
	};
	class Substitution
	{
	public:
		Substitution( Color c,Color s )
			:
			chroma( c ),
			sub( s )
		{}
		void operator()( Color cSrc,int xDest,int yDest,Graphics& gfx ) const
		{
			if( cSrc != chroma )
			{
				gfx.PutPixel( xDest,yDest,sub );
			}
		}
	private:
		Color chroma = Colors::Magenta;
		Color sub;
	};
	class Copy
	{
	public:
		void operator()( Color cSrc,int xDest,int yDest,Graphics& gfx ) const
		{
			gfx.PutPixel( xDest,yDest,cSrc );
		}
	};
	class Ghost
	{
	public:
		Ghost( Color c )
			:
			chroma( c )
		{}
		void operator()( Color src,int xDest,int yDest,Graphics& gfx ) const
		{
			if( src != chroma )
			{
				const Color dest = gfx.GetPixel( xDest,yDest );
				const Color blend = {
					unsigned char( (src.GetR() + dest.GetR()) / 2 ),
					unsigned char( (src.GetG() + dest.GetG()) / 2 ),
					unsigned char( (src.GetB() + dest.GetB()) / 2 )
				};
				gfx.PutPixel( xDest,yDest,blend );
			}
		}
	private:
		Color chroma;
	};
	// dissolves image by scanline and blends drawn pixels with a color
	// good for dying enemies i guess
	class DissolveHalfTint
	{
	public:
		DissolveHalfTint( Color chroma,Color tint,float percent )
			:
			chroma( chroma ),
			// divide channels by 2 via shift, mask to prevent bleeding between channels
			tint_pre( (tint.dword >> 1u) & 0b01111111011111110111111101111111u ),
			filled( int( float( height ) * percent ) )
		{}
		void operator()( Color src,int xDest,int yDest,Graphics& gfx ) const
		{
			// height mask determines frequency of vertical dissolve sections
			if( src != chroma && (yDest & height_mask) < filled )
			{
				const Color blend = tint_pre.dword +
					// divide channels by 2 via shift, mask to prevent bleeding between channels
					((src.dword >> 1u) & 0b01111111011111110111111101111111u);					
				gfx.PutPixel( xDest,yDest,blend );
			}
		}
	private:
		Color chroma;
		Color tint_pre;
		static constexpr int height = 4;
		static constexpr int height_mask = height - 1;
		int filled;
	};
	// blends sprite with whatever is on the screen
	// using the per-pixel alpha stored in the src pixels
	class AlphaBlendBaked : public Driver
	{
	public:
		class Functor
		{
		public:
			void operator()( Color src,int xDest,int yDest,Graphics& gfx ) const
			{
				// pre-extract alpha complement
				const int cAlpha = 255 - src.GetA();
				// reject drawing pixels if alpha == 0 (full transparent)
				// this will give a huge speedup if there are large sections
				// of blank space (pretty common), but slower if unpredictable
				// patters of transparency in a 50/50 mix (not really forseeable)
				if( cAlpha != 255 )
				{
					const Color dst = gfx.GetPixel( xDest,yDest );
					// blend channels by linear interpolation using integer math
					// (basic idea: src * alpha + dst * (1.0 - alpha), where alpha is from 0 to 1
					// we divide by 256 because it can be done with bit shift
					// it gives us at most 0.4% error, but this is negligible
					// optimized version has alpha premultiplied in src, all we need to do is
					// scale dst by calpha and then pack back into dword and add to src dword
					// there will be no overflow between channels because alpha + calpha == 255
					//
					// we can multiply the red and blue channels together in one operation
					// because the results will not overflow into neighboring channels
					// after, we shift to divide, and then mask to clear out the shifted garbo
					// channels are left in their byte position for easy combining with an add
					const int rb = (((dst.dword & 0xFF00FFu) * cAlpha) >> 8) & 0xFF00FFu;
					const int g = (((dst.dword & 0x00FF00u) * cAlpha) >> 8) & 0x00FF00u;
					// add multiplied dst channels together with premultiplied src channels
					// and write the resulting interpolated color to the screen
					gfx.PutPixel( xDest,yDest,rb + g + (src.dword & 0xFFFFFFu) );
				}
			}
		};
	public:
		void DrawSprite( Graphics& gfx,const Vei2& pos,const RectI& srcRect,const RectI& clip,const Surface& s,bool reversed = false ) const override
		{
			gfx.DrawSprite( pos.x,pos.y,srcRect,clip,s,Functor{},reversed );
		}
	};
	// does substitution effect using alpha instead of chroma to
	// mask the substitution
	class AlphaBakedSubstitution : public Driver
	{
	public:
		class Functor
		{
		public:
			Functor( Color sub )
				:
				sub_rb( sub.dword & 0x00FF00FFu ),
				sub_xg( sub.dword & 0xFF00FF00u )
			{}
			void operator()( Color src,int xDest,int yDest,Graphics& gfx ) const
			{
				// pre-extract alpha
				const int alpha = src.GetA();
				// reject drawing pixels if alpha == 0 (full transparent)
				if( alpha != 0 )
				{
					const int cAlpha = 255 - alpha;
					const Color dst = gfx.GetPixel( xDest,yDest );
					// blend channels by linear interpolation using integer math
					// dst
					const int rb_dest = (((dst.dword & 0xFF00FFu) * cAlpha) >> 8) & 0xFF00FFu;
					const int g_dest = (((dst.dword & 0x00FF00u) * cAlpha) >> 8) & 0x00FF00u;
					// src
					const int rb_src = ((sub_rb * alpha) >> 8) & 0x00FF00FFu;
					const int xg_src = (((size_t)sub_xg * alpha) >> 8) & 0xFF00FF00u;
					// add multiplied dst channels together with multiplied src channels
					// and write the resulting interpolated color to the screen
					gfx.PutPixel( xDest,yDest,rb_dest + g_dest + rb_src + xg_src );
				}
			}
		private:
			// presplit rb/g channels of substitution color
			unsigned int sub_rb;
			unsigned int sub_xg;
		};
	public:
		AlphaBakedSubstitution( Color sub )
			:
			sub( sub )
		{}
		void DrawSprite( Graphics& gfx,const Vei2& pos,const RectI& srcRect,const RectI& clip,const Surface& s,bool reversed = false ) const override
		{
			gfx.DrawSprite( pos.x,pos.y,srcRect,clip,s,Functor{ sub },reversed );
		}
	private:
		Color sub;
	};
}