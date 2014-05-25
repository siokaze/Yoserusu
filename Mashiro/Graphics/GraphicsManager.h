#ifndef INCLUDE_MASHIRO_GRAPHICS_MANAGER_INCLUDE_H_
#define INCLUDE_MASHIRO_GRAPHICS_MANAGER_INCLUDE_H_

#include "Mashiro/Graphics/Enum.h"

namespace Mashiro {
	namespace Math {
		class Matrix;
		class Vector4;
		class Vector3;
		class Vector2;
	}
}

namespace Mashiro{
namespace Graphics{

class Texture;
class Bitmap;
class VertexBuffer;
class IndexBuffer;
class Shader;
class RenderTarget;

class Manager{
public:
	Manager();
	static Manager instance();

	void end();
	//設定したテクスチャ、頂点バッファ、インデックスバッファで描画
	void drawIndexed( int indexCount, int start, PrimitiveType = PRIMITIVE_TRIANGLE );
	//設定したテクスチャ、頂点バッファで描画
	void draw( PrimitiveType = PRIMITIVE_TRIANGLE );

	//レンダリングターゲットテクスチャーセット
	void setTexture( RenderTarget targetTexture, int stage );
	//テクスチャーセット
	void setTexture( Texture, int stage );
	//頂点バッファセット
	void setVertexBuffer( VertexBuffer );
	//インデックスバッファセット
	void setIndexBuffer( IndexBuffer );
	//シェーダセット
	void setShader( Shader shader );
	//レンダリングターゲット [引数] 枚数, depthターゲットも使うかどうか？
	void setRenderTarget( int num, RenderTarget target[] );
	//バックバッファーのレンダリングターゲットに戻す
	void setBackBufferTarget();

	//Worldマトリックスセット
	void setWorldMatrix( const Math::Matrix& );
	//Projectionマトリックスセット
	void setProjectionMatrix( const Math::Matrix& );
	//Viewマトリックスセット
	void setViewMatrix( const Math::Matrix& );
	//ライト位置セット
	void setLight( const Math::Vector4& );
	//視線セット
	void setEyePos( const Math::Vector4& );
	//Diffuseカラーセット
	void setDiffuse( const Math::Vector4& );
	//透過率
	void setTransparency( float );
	//Ambientカラーセット
	void setAmbient( const Math::Vector4& );
	//スペキュラー
	void setSpecular( const Math::Vector4& );

	//ビューポートセット
	void setViewPort( int x, int y, int w, int h );
	//ビューポート取得
	void getViewPort( int* x, int* y, int* w, int* h );
	//ウィンドウ化
	void enableWindowScreen( bool );
	//ブレンドモード
	void setBlendMode( BlendMode );
	//ガリングモード
	void setCullMode( CullMode );

	void enableAlphaBlend( bool );
	void enableDepthWriteMask( bool );
	void enableDepthTest( bool );
	void enableDepthWrite( bool );
	
	int width() const;
	int height() const;
	
	static void create( void* hwnd, int w, int h, bool window );
	static void destroy();
	void getPointerModifier( float* scale, Mashiro::Math::Vector2* offset ) const;
};

}
}

#endif