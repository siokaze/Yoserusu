#ifndef INCLUDE_MASHIRO_GRAPHICS_IMPL_MANAGER_H_
#define INCLUDE_MASHIRO_GRAPHICS_IMPL_MANAGER_H_



#include "Mashiro/Mashiro.h"
#include "Mashiro/Graphics/Enum.h"
#include "Mashiro/Graphics/Texture.h"
#include "Mashiro/Graphics/src/VertexBufferImpl.h"
#include "Mashiro/Graphics/src/TextureImpl.h"
#include "Mashiro/Graphics/src/IndexBufferImpl.h"
#include "Mashiro/Graphics/src/ShaderImpl.h"
#include "Mashiro/Graphics/src/RenderTargetImpl.h"
#include "Mashiro/Graphics/src/BitmapImpl.h"
#include "Mashiro/Math/Matrix.h"
#include "Mashiro/Math/Vector3.h"
#include "Mashiro/Math/Vector4.h"

#include "Mashiro/Graphics/src/Release.h"

using namespace Mashiro::Math;

/*�V�����V�F�[�_��ǉ�����Ƃ��̓R���p�C�����Ă�������
[�g����]
fxc.exe �u���̓t�@�C�����v /T �u�V�F�[�_�[�̎�ށv /E �u�֐����v /Fh �u�o�̓t�@�C�����v
[��]
fxc.exe vs.fx /T vs_4_0 /E vs_main /Fh vs.h
*/

namespace Mashiro{
namespace Graphics {

class GraphicsManagerImpl{
public:
	GraphicsManagerImpl( HWND wh, int w, int h, bool window ) : 
	mDevice( 0 ),
	mRenderTargetView( 0 ),
	mDepthView( 0 ),
	mChain( 0 ),
	mWidth( w ),
	mHeight( h ),
	mCurrentVertexBuffer( 0 ),
	mCurrentIndexBuffer( 0 ),
	mCurrentTexture( 0 ),
	mCurrentRenderTargetTexture( 0 ),
	mRasterizerState( NULL ),
	mCurrentShader( 0 ){
		HRESULT hr;

		if( FAILED( hr = create( wh, w, h, window ) ) ){
			STRONG_ASSERT( hr != E_FAIL && "�f�o�C�X�����Ɏ��s���܂���" );
		}

		if( FAILED( hr = initBackBuffer() ) ){
			STRONG_ASSERT( hr != E_FAIL && "�o�b�N�o�b�t�@�̐����Ɏ��s���܂���" );
		}

		if( FAILED( hr = initDepthStencil( w, h ) ) ){
			STRONG_ASSERT( hr != E_FAIL && "�f�v�X�o�b�t�@�̐����Ɏ��s���܂���" );
		}
		//�f�o�C�X�ƃ^�[�Q�b�g�r���[�̌���
		mDevice->OMSetRenderTargets( 1, &mRenderTargetView, mDepthView );

		//�u�����h�X�e�[�g
		if( FAILED( hr = initBlendState( BLEND_LINER ) ) ){
			STRONG_ASSERT( hr != E_FAIL && "�u�����h�X�e�[�g�̐ݒ�Ɏ��s" );
		}
		//���X�^���C�U�ݒ�
		if( FAILED( hr = initRasterizer() ) ){
			STRONG_ASSERT( hr != E_FAIL && "���X�^���C�U�[�̐ݒ�Ɏ��s" );
		}
		//�x�[�V�b�N�R���X�^���g�o�b�t�@�쐬
		if( FAILED( hr = createBasicConstantBuffer() ) ){
			STRONG_ASSERT( hr != E_FAIL && "�R���X�^���g�o�b�t�@�̍쐬�Ɏ��s" );
		}
		
		//�����r���[�{�[�h�ݒ�(���ƂŕύX����)
		mViewPort.Height = h;
		mViewPort.Width = w;
		mViewPort.MinDepth = 0.0f;
		mViewPort.MaxDepth = 1.0f;
		mViewPort.TopLeftX = 0;
		mViewPort.TopLeftY = 0;
		mDevice->RSSetViewports( 1, &mViewPort );

		//WhiteTexture�̐���
		mWhiteTexture = NEW Texture::Impl( 1, 1, mDevice );
		int pitch;
		unsigned* data;
		mWhiteTexture->lock( &data, &pitch, 0 );
		*data = 0xffffffff;
		mWhiteTexture->unLock();
		data = 0;
	}

	void setViewport( int x, int y, int w, int h ){
		mViewPort.TopLeftX = static_cast< DWORD >( x );
		mViewPort.TopLeftY = static_cast< DWORD >( y );
		mViewPort.Width = static_cast< DWORD >( w );
		mViewPort.Height = static_cast< DWORD >( h );
		//�ݒ肩��r���[�{�[�g����낤
		float dw = static_cast< float >( mWidth );
		float dh = static_cast< float >( mHeight );
		float rw = static_cast< float >( mWidth );
		float rh = static_cast< float >( mHeight );
		float wRatio = dw / rw;
		float hRatio = dh / rh;
		float newX, newY, newW, newH;
		if( wRatio > hRatio ){//�c�ɂ��킹��
			newX = ( dw - hRatio * rw ) * 0.5f + static_cast< float >( x ) * hRatio;
			newY = 0.f;
			newW = static_cast< float >( w ) * hRatio;
			newH = dh;
		} else { //���ɂ��킹��
			newX = 0.f;
			newY = ( dh - wRatio * rh ) * 0.5f + static_cast< float >( y ) * wRatio;
			newW = dw;
			newH = static_cast< float >( h ) * wRatio;
		}
		D3D10_VIEWPORT vp;
		vp.MaxDepth = mViewPort.MaxDepth;
		vp.MinDepth = mViewPort.MinDepth;
		vp.Width = static_cast< DWORD >( newW + 0.5f );
		vp.Height = static_cast< DWORD >( newH + 0.5f );
		vp.TopLeftX = static_cast< DWORD >( newX );
		vp.TopLeftY = static_cast< DWORD >( newY );
		mDevice->RSSetViewports( 1, &vp );
	}
	void getViewport( int* x, int* y, int* w, int* h ){
		if( x ){
			*x = mViewPort.TopLeftX;
		}
		if( y ){
			*y = mViewPort.TopLeftY;
		}
		if( w ){
			*w = mViewPort.Width;
		}
		if( h ){
			*h = mViewPort.Height;
		}
	}
	~GraphicsManagerImpl(){
		setTextureRender( 0, 0 );
		setTexture( 0, 0 );
		setIndexBuffer( 0 );
		setVertexBuffer( 0 );
		setShader( 0 );

		SafeRelease( &mRenderTargetView );
		SafeRelease( &mRasterizerState );
		SafeRelease( &mDepthStencilTexture );
		SafeRelease( &mDepthView );
		SafeRelease( &mDepthStencil );
		SafeRelease( &mBlendState );
		SafeRelease( &mConstantBuffer );
		SafeRelease( &mChain );
		SafeRelease( &mDevice );

		SAFE_DELETE( mWhiteTexture );
	}
	void setBackBufferTarget(){
		//�`��^�[�Q�b�g�Z�b�g
		mDevice->OMSetRenderTargets( 1, &mRenderTargetView, mDepthView );
		float clearColor[] = { 0, 0, 0, 1 };
		mDevice->ClearRenderTargetView( mRenderTargetView, clearColor );
		mDevice->ClearDepthStencilView(
			mDepthView,
			D3D10_CLEAR_DEPTH | D3D10_CLEAR_STENCIL,   // �N���A����o�b�t�@
			1.0f,        // �[�x�̃N���A�l
			0         // �X�e���V���̃N���A�l
			);
	}
	void end(){
        mChain->Present( 0, 0 );
	}
	void draw( PrimitiveType prim = PRIMITIVE_TRIANGLE ){
		//�r���[�ݒ�
		setViewport( mViewPort.TopLeftX, mViewPort.TopLeftY, mViewPort.Width, mViewPort.Height );
		//�e�N�X�`���[�Z�b�g����Ă�H
		if( !mCurrentTexture && !mCurrentRenderTargetTexture ){
			setTexture( mWhiteTexture, 0 );
		}
		// �v���~�e�B�u��`
		D3D10_PRIMITIVE_TOPOLOGY  dxPrim = D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;
		switch( prim ){
			case PRIMITIVE_TRIANGLE: dxPrim = D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP; break;
			case PRIMITIVE_LINE: dxPrim = D3D10_PRIMITIVE_TOPOLOGY_LINESTRIP; break;
			case PRIMITIVE_POINT: dxPrim = D3D10_PRIMITIVE_TOPOLOGY_POINTLIST; break;
		}
	    mDevice->IASetPrimitiveTopology( dxPrim );

		STRONG_ASSERT( mCurrentVertexBuffer && "VertexBuffer is not set." );
		STRONG_ASSERT( mCurrentShader && "Shader is not set." );
		//�V�F�[�_�Z�b�g
		mCurrentShader->setShader( mDevice );
		//���_�o�b�t�@�Z�b�g		
		UINT stride = mCurrentVertexBuffer->mSize;
	    UINT offset = 0;
		mDevice->IASetVertexBuffers( 
			0, 
			1, 
			&mCurrentVertexBuffer->mVertexBuffer,
			&stride, &offset);
		int vertexNumber = mCurrentVertexBuffer->mVertexNumber;
		//�R���X�^���g�o�b�t�@�Z�b�g
		BasicConstantBuffer* inBuffer = NULL;
		if( SUCCEEDED( mConstantBuffer->Map(D3D10_MAP_WRITE_DISCARD,NULL,(void**)&inBuffer))){
			inBuffer->mWorld = mBasicConstantBuffer.mWorld;
			inBuffer->mView = mBasicConstantBuffer.mView;
			inBuffer->mProj = mBasicConstantBuffer.mProj;
			inBuffer->mSpecular = mBasicConstantBuffer.mSpecular;
			inBuffer->mLight = mBasicConstantBuffer.mLight;
			inBuffer->mEye = mBasicConstantBuffer.mEye;
			inBuffer->mDiffuse = mBasicConstantBuffer.mDiffuse;
			inBuffer->mAmbient = mBasicConstantBuffer.mAmbient;
			mConstantBuffer->Unmap();
		}
		mDevice->VSSetConstantBuffers( 0, 1, &mConstantBuffer );
		mDevice->PSSetConstantBuffers( 0, 1, &mConstantBuffer );
		mDevice->GSSetConstantBuffers( 0, 1, &mConstantBuffer );
		mDevice->Draw( vertexNumber, 0 );
	}
	void drawIndexed( int indexCount, int start, PrimitiveType prim = PRIMITIVE_TRIANGLE ){
		//�e�N�X�`���[�Z�b�g����Ă�H
		if( !mCurrentTexture && !mCurrentRenderTargetTexture ){
			setTexture( mWhiteTexture, 0 );
		}
		//�r���[�ݒ�
		setViewport( mViewPort.TopLeftX, mViewPort.TopLeftY, mViewPort.Width, mViewPort.Height );

		// �v���~�e�B�u��`
		D3D10_PRIMITIVE_TOPOLOGY  dxPrim = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		switch( prim ){
			case PRIMITIVE_TRIANGLE: dxPrim = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST; break;
			case PRIMITIVE_LINE: dxPrim = D3D10_PRIMITIVE_TOPOLOGY_LINESTRIP; break;
			case PRIMITIVE_POINT: dxPrim = D3D10_PRIMITIVE_TOPOLOGY_POINTLIST; break;
		}
	    mDevice->IASetPrimitiveTopology( dxPrim );

		STRONG_ASSERT( mCurrentVertexBuffer && "VertexBUffer is not set." );
		STRONG_ASSERT( mCurrentIndexBuffer && "IndexBuffer is not set." );
		STRONG_ASSERT( mCurrentShader && "Shader is not set." );
		//�V�F�[�_�Z�b�g
		mCurrentShader->setShader( mDevice );
		//���_�o�b�t�@�Z�b�g
		UINT stride = mCurrentVertexBuffer->mSize;
	    UINT offset = 0;
		mDevice->IASetVertexBuffers( 
			0, 
			1, 
			&mCurrentVertexBuffer->mVertexBuffer,
			&stride, &offset);
		//�C���f�b�N�X�o�b�t�@�Z�b�g
		mDevice->IASetIndexBuffer(
			mCurrentIndexBuffer->mIndexBuffer,
			DXGI_FORMAT_R32_UINT,
			0 );
		//�R���X�^���g�o�b�t�@�Z�b�g
		BasicConstantBuffer* inBuffer = NULL;
		if( SUCCEEDED( mConstantBuffer->Map(D3D10_MAP_WRITE_DISCARD,NULL,(void**)&inBuffer))){
			inBuffer->mWorld = mBasicConstantBuffer.mWorld;
			inBuffer->mView = mBasicConstantBuffer.mView;
			inBuffer->mProj = mBasicConstantBuffer.mProj;
			inBuffer->mSpecular = mBasicConstantBuffer.mSpecular;
			inBuffer->mLight = mBasicConstantBuffer.mLight;
			inBuffer->mEye = mBasicConstantBuffer.mEye;
			inBuffer->mDiffuse = mBasicConstantBuffer.mDiffuse;
			inBuffer->mAmbient = mBasicConstantBuffer.mAmbient;
			mConstantBuffer->Unmap();
		}
		mDevice->VSSetConstantBuffers( 0, 1, &mConstantBuffer );
		mDevice->PSSetConstantBuffers( 0, 1, &mConstantBuffer );
		mDevice->GSSetConstantBuffers( 0, 1, &mConstantBuffer );
		mDevice->DrawIndexed( indexCount, start, 0 );
	}
	void setRenderTarget( int num, RenderTarget::Impl* o[] ){
		//�`��^�[�Q�b�g�Z�b�g
		float clearColor[] = { 0, 0, 0, 1 };
		ID3D10RenderTargetView* target[ 8 ];
		for( int i = 0; i < num; ++i ){
			target[ i ] = o[ i ]->mRtv;
		}	
		//�Z�b�g
		mDevice->OMSetRenderTargets( num, target, mDepthView );
		for( int i = 0; i < num; ++i ){
			//�N���A�[
			mDevice->ClearRenderTargetView( target[ i ], clearColor );
			mDevice->ClearDepthStencilView(
				mDepthView,
				D3D10_CLEAR_DEPTH | D3D10_CLEAR_STENCIL,   // �N���A����o�b�t�@
				1.0f,        // �[�x�̃N���A�l
				0         // �X�e���V���̃N���A�l
				);
		}
	}
	void setVertexBuffer( VertexBuffer::Impl* o ){
		if( mCurrentVertexBuffer == o ){
			return;
		}
		if( mCurrentVertexBuffer ){
			mCurrentVertexBuffer->release();
			if( mCurrentVertexBuffer->referenceCount() == 0 ){
				SAFE_DELETE( mCurrentVertexBuffer );
			}
		}
		mCurrentVertexBuffer = o;
		if( o ){
			o->refer();
		}
	}
	void setIndexBuffer( IndexBuffer::Impl* o ){
		if( mCurrentIndexBuffer == o ){
			return;
		}
		if( mCurrentIndexBuffer ){
			mCurrentIndexBuffer->release();
			if( mCurrentIndexBuffer->referenceCount() == 0 ){
				SAFE_DELETE( mCurrentIndexBuffer );
			}
		}
		mCurrentIndexBuffer = o;
		if( o ){
			o->refer();
		}
	}
	void setTextureRender( RenderTarget::Impl* o, int stage ){
		if( mCurrentRenderTargetTexture == o ){
			return;
		}
		ID3D10ShaderResourceView* tex;
		tex = ( o ) ? o->mSrv : 0;
		mDevice->PSSetShaderResources( stage, 1, &tex );
		if( mCurrentRenderTargetTexture ){
			mCurrentRenderTargetTexture->release();
			if( mCurrentRenderTargetTexture->referenceCount() == 0 ){
				SAFE_DELETE( mCurrentRenderTargetTexture );
			}
		}
		mCurrentRenderTargetTexture = o;
		if( o ){
			o->refer();
		}
	}
	void setTexture( Texture::Impl* o, int stage ){
		if( mCurrentTexture == o ){
			return;
		}
		ID3D10ShaderResourceView* tex;
		tex = ( o ) ? o->mTexture : 0;
		mDevice->PSSetShaderResources( stage, 1, &tex );
		if( mCurrentTexture ){
			mCurrentTexture->release();
			if( mCurrentTexture->referenceCount() == 0 ){
				SAFE_DELETE( mCurrentTexture );
			}
		}
		mCurrentTexture = o;
		if( o ){
			o->refer();
		}
	}
	void setShader( Shader::Impl* o ){
		if( mCurrentShader == o ){
			return;
		}
		if( mCurrentShader ){
			mCurrentShader->release();
			if( mCurrentShader->referenceCount() == 0 ){
				SAFE_DELETE( mCurrentShader );
			}
		}
		mCurrentShader = o;
		if( o ){
			o->refer();
		}
	}
	void getPointerModifier( float* scale, Vector2* offset ){
		float dw = static_cast< float >( mWidth );
		float dh = static_cast< float >( mHeight );
		float rw = static_cast< float >( mWidth );
		float rh = static_cast< float >( mHeight );
		float wRatio = rw / dw;
		float hRatio = rh / dh;
		if ( wRatio > hRatio ){ //���ɂ��킹��B
			*scale = wRatio;
			offset->x = 0.f;
			offset->y = ( dh - ( rh / wRatio ) ) * -0.5f;
		}else{ //�c�ɂ��킹��
			*scale = hRatio;
			offset->x = ( dw - ( rw / hRatio ) ) * -0.5f;
			offset->y = 0.f;
		}
	}
	void setCullMode( CullMode c ){
		D3D10_RASTERIZER_DESC desc;
		mRasterizerState->GetDesc( &desc );
		if( c == CULL_MODE_NONE ){
			desc.CullMode = D3D10_CULL_NONE;
		} else if( c == CULL_MODE_FRONT ){
			desc.CullMode = D3D10_CULL_FRONT;
		} else if( c == CULL_MODE_BACK ){
			desc.CullMode = D3D10_CULL_BACK;
		}
		HRESULT hr;

		ID3D10RasterizerState* state;
		hr = mDevice->CreateRasterizerState( &desc, &state );

		mDevice->CreateRasterizerState( &desc, &mRasterizerState );
		mDevice->RSSetState( mRasterizerState );

		SafeRelease( &state );
	}
	void enableDepthWriteMask( bool r ){
		D3D10_DEPTH_STENCIL_DESC desc;
		mDepthStencil->GetDesc( &desc );
		if( r ){
			desc.DepthWriteMask = D3D10_DEPTH_WRITE_MASK_ALL;
		} else {
			desc.DepthWriteMask = D3D10_DEPTH_WRITE_MASK_ZERO;
		}
		HRESULT hr;
		hr = mDevice->CreateDepthStencilState( &desc, &mDepthStencil );
		if( FAILED( hr ) ){
			hr = E_FAIL;
			STRONG_ASSERT( hr != E_FAIL && "DepthTest Error" );
		}
		mDevice->OMSetDepthStencilState( mDepthStencil, 0 );
	}
	void setDepthTest( bool t ){
		D3D10_DEPTH_STENCIL_DESC desc;
		mDepthStencil->GetDesc( &desc );
		BOOL tf = ( t ) ? TRUE : FALSE;
		if( desc.DepthEnable == tf ){
			return;
		}
		desc.DepthEnable = tf;
		HRESULT hr;

		ID3D10DepthStencilState* state;
		hr = mDevice->CreateDepthStencilState( &desc, &state );
		if( FAILED( hr ) ){
			hr = E_FAIL;
			STRONG_ASSERT( hr != E_FAIL && "DepthTest Error" );
		}
		mDevice->OMSetDepthStencilState( state, 0 );

		SafeRelease( &state );
	}
	void setDepthWrite( bool w ){
		D3D10_DEPTH_STENCIL_DESC desc;
		mDepthStencil->GetDesc( &desc );
		BOOL tf = ( w ) ? TRUE : FALSE;
		if( desc.StencilEnable == tf ){
			return;
		}

		desc.StencilEnable = tf;
		HRESULT hr;

		ID3D10DepthStencilState* state;
		hr = mDevice->CreateDepthStencilState( &desc, &state );
		if( FAILED( hr ) ){
			hr = E_FAIL;
			STRONG_ASSERT( hr != E_FAIL && "DepthWrite Error" );
		}
		mDevice->OMSetDepthStencilState( state, 0 );

		SafeRelease( &state );
	}
	void enableAlphaBlend( bool r ){
		HRESULT hr = S_OK;
		D3D10_BLEND_DESC desc;
		mBlendState->GetDesc( &desc );
		if( !r ){
			desc.BlendEnable[ 0 ] = FALSE;
		} else if( r ){
			desc.BlendEnable[ 0 ] = TRUE;
		}
		hr = mDevice->CreateBlendState( &desc, &mBlendState );
		if( FAILED( hr ) ){
			hr = E_FAIL;
			STRONG_ASSERT( hr != E_FAIL && "Blend State�̐ݒ�Ɏ��s" );
		}
		mDevice->OMSetBlendState( mBlendState, D3DXVECTOR4( 0.f, 0.f, 0.f, 0.f ), 0xffffffff );
	}
	void setBlendMode( BlendMode s ){
		// �u�����h�X�e�[�g�ݒ�
		HRESULT hr = S_OK;
		D3D10_BLEND_DESC desc;
		mBlendState->GetDesc( &desc );
		switch( s ){
		case BLEND_NONE:
			 desc.BlendEnable[ 0 ] = FALSE;
			 desc.SrcBlend = D3D10_BLEND_ONE;
			 desc.DestBlend = D3D10_BLEND_ONE;
			 desc.BlendOp = D3D10_BLEND_OP_ADD;
		case BLEND_ADD:
			desc.BlendEnable[ 0 ] = TRUE;
			desc.SrcBlend = D3D10_BLEND_ONE;
			desc.DestBlend = D3D10_BLEND_ONE;
			desc.BlendOp = D3D10_BLEND_OP_ADD;
			break;
		case BLEND_LINER:
			desc.BlendEnable[ 0 ] = TRUE;
			desc.SrcBlend = D3D10_BLEND_SRC_ALPHA;
			desc.DestBlend = D3D10_BLEND_INV_SRC_ALPHA;
			desc.BlendOp = D3D10_BLEND_OP_ADD;
			break;
		case BLEND_SUB:
			desc.BlendEnable[ 0 ] = TRUE;
			desc.SrcBlend = D3D10_BLEND_SRC_ALPHA;
			desc.DestBlend = D3D10_BLEND_ONE;
			desc.BlendOp = D3D10_BLEND_OP_SUBTRACT;
			break;
		case BLEND_MUL:
			desc.BlendEnable[ 0 ] = TRUE;
			desc.SrcBlend = D3D10_BLEND_ZERO;
			desc.DestBlend = D3D10_BLEND_SRC_COLOR;
			desc.BlendOp = D3D10_BLEND_OP_MAX;			
			break;
		}
		hr = mDevice->CreateBlendState( &desc, &mBlendState );
		if( FAILED( hr ) ){
			hr = E_FAIL;
			STRONG_ASSERT( hr != E_FAIL && "Blend State�̐ݒ�Ɏ��s" );
		}
		mDevice->OMSetBlendState( mBlendState, D3DXVECTOR4( 0.f, 0.f, 0.f, 0.f ), 0xffffffff );
	}
	ID3D10Device1* dxDevice() const {
		return mDevice;
	}
	IDXGISwapChain* chain(){
		return mChain;
	}
	HRESULT create( const HWND hWnd, const int width, const int height, bool window ){
		IDXGIDevice* pDXGIDevice = NULL;
		IDXGIAdapter* pAdapter = NULL;
		IDXGIFactory* pDXGIFactory = NULL;

		if( FAILED( CreateD3DDevice( NULL, D3D10_DRIVER_TYPE_HARDWARE,
			D3D10_CREATE_DEVICE_BGRA_SUPPORT, &mDevice)))
		{
			// Direct3DDevice�̏�����(�\�t�g�E�F�A�Ő���)
			if (FAILED( CreateD3DDevice( NULL, D3D10_DRIVER_TYPE_WARP,
					D3D10_CREATE_DEVICE_BGRA_SUPPORT, &mDevice)))
			{
				return E_FAIL;
			}
		}

		if( FAILED( mDevice->QueryInterface( &pDXGIDevice ) ) ){
			return E_FAIL;
		}

		if( FAILED( pDXGIDevice->GetAdapter( &pAdapter ) ) ){
			return E_FAIL;
		}
		if( FAILED( pAdapter->GetParent( IID_PPV_ARGS( &pDXGIFactory ) ) ) ){
			return E_FAIL;
		}

		//DXGI_SWAP_CHAIN_DESC�\����
		DXGI_SWAP_CHAIN_DESC swapDesc;
		::ZeroMemory(&swapDesc, sizeof(swapDesc));

		swapDesc.BufferDesc.Width = mWidth;
		swapDesc.BufferDesc.Height = mHeight;
		swapDesc.BufferDesc.RefreshRate.Numerator = 60;
		swapDesc.BufferDesc.RefreshRate.Denominator = 1;
		swapDesc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		swapDesc.SampleDesc.Count = 1;
		swapDesc.SampleDesc.Quality = 0;
		swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapDesc.BufferCount = 1;
		swapDesc.OutputWindow = hWnd;
		swapDesc.Windowed = window;      // �E�B���h�E���[�h
		//�f�o�C�X�̍쐬
		if( FAILED( pDXGIFactory->CreateSwapChain( mDevice, &swapDesc, &mChain ) ) ){
				return E_FAIL;
		}

		//���
		SafeRelease( &pDXGIDevice );
		SafeRelease( &pAdapter );
		SafeRelease( &pDXGIFactory );

		return S_OK;
	}

	HRESULT CreateD3DDevice( 
		IDXGIAdapter* adapter,
		D3D10_DRIVER_TYPE driverType,
		unsigned int flags,
		ID3D10Device1 **ppDevice ){

		HRESULT hr = S_OK;

		const D3D10_FEATURE_LEVEL1 levelAttempts[] = {
			D3D10_FEATURE_LEVEL_10_1,
			D3D10_FEATURE_LEVEL_10_0,
			D3D10_FEATURE_LEVEL_9_3,
			D3D10_FEATURE_LEVEL_9_2,
			D3D10_FEATURE_LEVEL_9_1,
		};

		for( UINT level = 0; level < ARRAYSIZE( levelAttempts ); level++ ){
			ID3D10Device1* pDevice = NULL;
			hr = D3D10CreateDevice1( adapter, driverType, NULL, flags,
				levelAttempts[ level ], D3D10_1_SDK_VERSION, &pDevice );
			if( SUCCEEDED( hr ) ){
				*ppDevice = pDevice;
				pDevice = NULL;
				break;
			}
		}

		return hr;
	}
	HRESULT initBackBuffer(){
		ID3D10Texture2D* backBuffer;

		mChain->GetBuffer( 0, __uuidof( ID3D10Texture2D ), ( LPVOID* )&backBuffer );
		
		if( FAILED( mDevice->CreateRenderTargetView( backBuffer, 0, &mRenderTargetView ) ) ){
			return E_FAIL;
		}

		backBuffer->Release();
		backBuffer = 0;

		return S_OK;
	}

	HRESULT initDepthStencil( unsigned int width, unsigned int height ){
		D3D10_TEXTURE2D_DESC descDepth;
		ZeroMemory( &descDepth, sizeof( descDepth ) );
		// �[�x�X�e���V���e�N�X�`���𐶐�
		descDepth.Width = width;
		descDepth.Height = height;
		descDepth.MipLevels = 1;
		descDepth.ArraySize = 1;
		descDepth.Format = DXGI_FORMAT_D32_FLOAT;
		descDepth.SampleDesc.Count = 1;
		descDepth.SampleDesc.Quality = 0;
		descDepth.Usage = D3D10_USAGE_DEFAULT;
		descDepth.BindFlags = D3D10_BIND_DEPTH_STENCIL;
		descDepth.CPUAccessFlags = 0;
		descDepth.MiscFlags = 0;

		if( FAILED( mDevice->CreateTexture2D( &descDepth, NULL, &mDepthStencilTexture ) ) ){
			E_FAIL;
		}

		//�[�x�X�e���V���r���[�𐶐�
		D3D10_DEPTH_STENCIL_VIEW_DESC descDSV;
		ZeroMemory( &descDSV, sizeof( descDSV ) );
		descDSV.Format = descDepth.Format;
		descDSV.ViewDimension = D3D10_DSV_DIMENSION_TEXTURE2D;
		descDSV.Texture2D.MipSlice = 0;
		if( FAILED( mDevice->CreateDepthStencilView( mDepthStencilTexture, &descDSV, &mDepthView ) ) ){
			return E_FAIL;
		}

		//Z�o�b�t�@�ݒ�
		D3D10_DEPTH_STENCIL_DESC descSten;
		ZeroMemory(&descSten,sizeof(descSten));//������
		descSten.DepthEnable = true;
		descSten.DepthWriteMask = D3D10_DEPTH_WRITE_MASK_ALL;
		descSten.DepthFunc = D3D10_COMPARISON_LESS;
		descSten.StencilEnable = false;
		if( FAILED( mDevice->CreateDepthStencilState( &descSten, &mDepthStencil ) ) ){
			return E_FAIL;
		}
		mDevice->OMSetDepthStencilState( mDepthStencil, 0 );
		return S_OK;
	}

	HRESULT initBlendState( BlendMode s ){
		// �u�����h�X�e�[�g�ݒ�
		D3D10_BLEND_DESC desc;
		memset( &desc, 0, sizeof( desc ) );
		desc.AlphaToCoverageEnable = FALSE;
		for( int i = 0; i < 8; ++i ){
			desc.BlendEnable[ i ] = TRUE;
			desc.RenderTargetWriteMask[ i ] = D3D10_COLOR_WRITE_ENABLE_ALL;
		}
		desc.SrcBlend = D3D10_BLEND_SRC_ALPHA;
		desc.DestBlend = D3D10_BLEND_INV_SRC_ALPHA;
		desc.BlendOp	= D3D10_BLEND_OP_ADD;
		desc.BlendOpAlpha = D3D10_BLEND_OP_ADD;
		desc.DestBlendAlpha = D3D10_BLEND_ZERO;
		desc.SrcBlendAlpha = D3D10_BLEND_ONE;
		
		mBlendState = 0;
		if( FAILED( mDevice->CreateBlendState( &desc, &mBlendState ) ) ){
			return E_FAIL;
		}
		mDevice->OMSetBlendState( mBlendState, D3DXVECTOR4( 0.f, 0.f, 0.f, 0.f ), 0xffffffff );
		return S_OK;
	}

	HRESULT initRasterizer(){
		//���X�^���C�U�[����
		D3D10_RASTERIZER_DESC desc;
		ZeroMemory( &desc, sizeof( desc ) );
		desc.FillMode = D3D10_FILL_SOLID;
		desc.CullMode = D3D10_CULL_BACK;
		desc.FrontCounterClockwise = false;
		desc.DepthBias = 0;
		desc.DepthBiasClamp = 0.0f;
		desc.SlopeScaledDepthBias = 0.0f;
		desc.DepthClipEnable = true;
		desc.ScissorEnable = false;
		desc.MultisampleEnable = true;
		desc.AntialiasedLineEnable = true;
		if( FAILED(  mDevice->CreateRasterizerState( &desc, &mRasterizerState ) ) ){
			return E_FAIL;
		}
		mDevice->RSSetState( mRasterizerState );

		return S_OK;
	}
	HRESULT createBasicConstantBuffer(){
		D3D10_BUFFER_DESC cb;
		cb.BindFlags = D3D10_BIND_CONSTANT_BUFFER;
		cb.ByteWidth = sizeof( BasicConstantBuffer );
		cb.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;
		cb.MiscFlags = 0;
		cb.Usage = D3D10_USAGE_DYNAMIC;

		HRESULT hr = E_FAIL;

		//�R���X�^���g�o�b�t�@���쐬
		if( FAILED( hr = mDevice->CreateBuffer( &cb, NULL, &mConstantBuffer ) ) ){
			return E_FAIL;
		}
		return S_OK;
	}
	void setWorldMatrix( const Matrix& world ){
		Matrix mat;
		mat.setTranspose( world );
		mBasicConstantBuffer.mWorld = mat;
	}
	void setViewMatrix( const Matrix& view ){
		Matrix mat;
		mat.setTranspose( view );
		mBasicConstantBuffer.mView = mat;
	}
	void setProjectionMatrix( const Matrix& proj ){
		Matrix mat;
		mat.setTranspose( proj );
		mBasicConstantBuffer.mProj = mat;
	}
	void setLight( const Vector4& v ){
		mBasicConstantBuffer.mLight = v;
	}
	void setDiffuse( const Vector4& v ){
		mBasicConstantBuffer.mDiffuse = v;
	}
	void setEyePos( const Vector4& v ){
		mBasicConstantBuffer.mEye = v;
	}
	void setAmbient( const Vector4& v ){
		mBasicConstantBuffer.mAmbient = v;
	}
	void setSpecular( const Vector4& v ){
		mBasicConstantBuffer.mSpecular = v;
	}
	
	//DirectX10��
	ID3D10DepthStencilState* mDepthStencil;
	ID3D10RasterizerState* mRasterizerState;
	ID3D10Device1* mDevice;
	IDXGISwapChain* mChain;
	ID3D10RenderTargetView* mRenderTargetView;
	ID3D10DepthStencilView* mDepthView;
	ID3D10Texture2D* mDepthStencilTexture;
	ID3D10BlendState* mBlendState;
	D3D10_VIEWPORT mViewPort;
	
	//Buffer�S
	VertexBuffer::Impl* mCurrentVertexBuffer;
	IndexBuffer::Impl* mCurrentIndexBuffer;
	Texture::Impl* mCurrentTexture;
	Texture::Impl* mWhiteTexture;
	Shader::Impl* mCurrentShader;
	RenderTarget::Impl* mCurrentRenderTargetTexture;

	//��{�̃R���X�^���g�o�b�t�@
	ID3D10Buffer* mConstantBuffer;
	BasicConstantBuffer mBasicConstantBuffer;

	//�E�B���h�E�̏��
	int mWidth;
	int mHeight;
	unsigned mFrameId;
	HWND mWindowHandle;
};
extern GraphicsManagerImpl* gManagerImpl; //�C���X�^���X

}
}

#endif