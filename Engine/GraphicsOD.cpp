#define GOD_GRAPHICS
#include "Graphics.h"
#include "SpriteEffect.h"
#include "BloomProcessor.h"

void Graphics::BeginFrame( Color fillColor )
{
	// clear the sysbuffer
	sysBuffer.Fill( fillColor );
	cam.Generate();
}

void Graphics::EndFrame()
{
#ifdef BLOOM_ACTIVE
	pBp->Go();
#endif

	HRESULT hr;

	// lock and map the adapter memory for copying over the sysbuffer
	if( FAILED( hr = pImmediateContext->Map( pSysBufferTexture.Get(),0u,
		D3D11_MAP_WRITE_DISCARD,0u,&mappedSysBufferTexture ) ) )
	{
		throw CHILI_GFX_EXCEPTION( hr,L"Mapping sysbuffer" );
	}
	// setup parameters for copy operation
	constexpr int fringe = BloomProcessor::GetFringeSize();
	Color* pDst = reinterpret_cast<Color*>(mappedSysBufferTexture.pData);
	const size_t dstPitch = mappedSysBufferTexture.RowPitch / sizeof( Color );
	const size_t srcPitch = Graphics::ScreenWidth + fringe * 2;
	const size_t rowBytes = srcPitch * sizeof( Color );
	// perform the copy line-by-line (exclude fringe region)
	for( size_t y = 0u; y < Graphics::ScreenHeight; y++ )
	{
		memcpy( &pDst[y * dstPitch],&sysBuffer.Data()[(y + fringe) * srcPitch + fringe],rowBytes );
	}
	// release the adapter memory
	pImmediateContext->Unmap( pSysBufferTexture.Get(),0u );

	// render offscreen scene texture to back buffer
	pImmediateContext->IASetInputLayout( pInputLayout.Get() );
	pImmediateContext->VSSetShader( pVertexShader.Get(),nullptr,0u );
	pImmediateContext->PSSetShader( pPixelShader.Get(),nullptr,0u );
	pImmediateContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
	const UINT stride = sizeof( FSQVertex );
	const UINT offset = 0u;
	pImmediateContext->IASetVertexBuffers( 0u,1u,pVertexBuffer.GetAddressOf(),&stride,&offset );
	pImmediateContext->PSSetShaderResources( 0u,1u,pSysBufferTextureView.GetAddressOf() );
	pImmediateContext->PSSetSamplers( 0u,1u,pSamplerState.GetAddressOf() );
	pImmediateContext->Draw( 6u,0u );

	// flip back/front buffers
	if( FAILED( hr = pSwapChain->Present( 1u,0u ) ) )
	{
		if( hr == DXGI_ERROR_DEVICE_REMOVED )
		{
			throw CHILI_GFX_EXCEPTION( pDevice->GetDeviceRemovedReason(),L"Presenting back buffer [device removed]" );
		}
		else
		{
			throw CHILI_GFX_EXCEPTION( hr,L"Presenting back buffer" );
		}
	}
}

void Graphics::PutPixel( int x,int y,Color c )
{
	sysBuffer.PutPixel( 
		x + BloomProcessor::GetFringeSize() + cam.GetXOffset(),
		y + BloomProcessor::GetFringeSize() + cam.GetYOffset(),
		c 
	);
}

Color Graphics::GetPixel( int x,int y ) const
{
	return sysBuffer.GetPixel( 
		x + BloomProcessor::GetFringeSize() + cam.GetXOffset(),
		y + BloomProcessor::GetFringeSize() + cam.GetYOffset()
	);
}

void dummy( Graphics& gfx )
{
	gfx.DrawSprite( 0,0,{ 0,0,0,0 },{ 0,0,0,0 },{ 0,0 },SpriteEffect::Copy{} );
	gfx.DrawSprite( 0,0,{ 0,0,0,0 },{ 0,0,0,0 },{ 0,0 },SpriteEffect::Chroma{ Colors::Black } );
	gfx.DrawSprite( 0,0,{ 0,0,0,0 },{ 0,0,0,0 },{ 0,0 },SpriteEffect::Substitution{ Colors::Black,Colors::Black } );
	gfx.DrawSprite( 0,0,{ 0,0,0,0 },{ 0,0,0,0 },{ 0,0 },SpriteEffect::Ghost{ Colors::Black } );
	gfx.DrawSprite( 0,0,{ 0,0,0,0 },{ 0,0,0,0 },{ 0,0 },SpriteEffect::DissolveHalfTint{ Colors::Black,Colors::Black,0.0f } );
	SpriteEffect::AlphaBlendBaked{}.DrawSprite( gfx,{ 0,0 },{ 0,0,0,0 },{ 0,0,0,0 },{ 0,0 } );
	SpriteEffect::AlphaBakedSubstitution{ Colors::Black }.DrawSprite( gfx,{ 0,0 },{ 0,0,0,0 },{ 0,0,0,0 },{ 0,0 } );
}