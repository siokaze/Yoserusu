#ifndef INCLUDE_MASHIRO_GRAPHICS_MODELIMPL_H_
#define INCLUDE_MASHIRO_GRAPHICS_MODELIMPL_H_

#include "Mashiro/Mashiro.h"
#include "Mashiro/WorkSpace/WorkSpace.h"
#include "Mashiro/Base/Impl/ReferenceType.h"
#include "Mashiro/Math/Functions.h"
#include "Mashiro/Math/Vector3.h"
#include "Mashiro/Math/Matrix.h"
#include "Mashiro/Graphics/GraphicsManager.h"
#include "Mashiro/Scene/Model.h"
#include "Mashiro/Graphics/IndexBuffer.h"
#include "Mashiro/Graphics/VertexBuffer.h"
#include "Mashiro/Graphics/Texture.h"
#include "Mashiro/Graphics/RenderTarget.h"
#include "Mashiro/Scene/src/FBXFileLoaderImpl.h"
#include "Mashiro/Scene/src/PMDFileLoaderImpl.h"
#include "Mashiro/Scene/src/MRSFileLoaderImpl.h"
#include <sstream>
#include <vector>
using namespace std;
using namespace Mashiro::Graphics;

namespace Mashiro{

namespace Scene {
class Model::Impl : public Mashiro::ReferenceType {
public:
	Impl( PMDFileLoader::Impl* data ) : 
	mPosition( 0.f ),
	mAngle( 0.f ),
	mScale( 1.f ),
	mColor( 1.f ),
	mMaterialCount( data->material_count ),
	mTrans( 1.f ),
	mTexMaterialNumber( -1 ),
	mRenderTexMaterialNumber( -1 ){
		mWorld.setIdentity();
		mVertexBuffer = data->mVertexBuffer;
		mIndexBuffer = NEW IndexBuffer[ data->material_count ];
		mMaterial = NEW Material[ data->material_count ];
		for( unsigned long i = 0; i < data->material_count; ++i ){
			mIndexBuffer[ i ] = data->mIndexBuffer[ i ];
			mMaterial[ i ].mAmbient = data->mMaterial[ i ].mAmbient;
			mMaterial[ i ].mDiffuse = data->mMaterial[ i ].mDiffuse;
			mMaterial[ i ].mSpecular = data->mMaterial[ i ].mSpecular;
			mMaterial[ i ].mTexture = data->mMaterial[ i ].mTexture;
			mMaterial[ i ].mNumFace = data->mMaterial[ i ].mNumFace;
		}
	}
	Impl( FBXFileLoader::Impl* data ) : 
	mPosition( 0.f ),
	mAngle( 0.f ),
	mScale( 1.f ),
	mColor( 1.f ),
	mMaterialCount( data->mNumMaterial ),
	mTrans( 1.f ),
	mTexMaterialNumber( -1 ),
	mRenderTexMaterialNumber( -1 ){
		mWorld.setIdentity();
		//データを移していく
		mVertexBuffer = data->mVertex;
		mIndexBuffer = NEW IndexBuffer[ mMaterialCount ];
		mMaterial = NEW Material[ mMaterialCount ];
		for( unsigned long i = 0; i < mMaterialCount; ++i ){
			mIndexBuffer[ i ] = data->mIndex[ i ];
			mMaterial[ i ].mAmbient = data->mMaterial[ i ].mAmbient;
			mMaterial[ i ].mDiffuse = data->mMaterial[ i ].mDiffuse;
			mMaterial[ i ].mSpecular = data->mMaterial[ i ].mSpecular;
			mMaterial[ i ].mTexture = data->mMaterial[ i ].mTexture;
			mMaterial[ i ].mNumFace = data->mMaterial[ i ].mNumFace;
		}
	}
	Impl( MRSFileLoader::Impl* data ) : 
	mPosition( 0.f ),
	mAngle( 0.f ),
	mScale( 1.f ),
	mColor( 1.f ),
	mMaterialCount( data->mMaterialCount ),
	mTrans( 1.f ),
	mTexMaterialNumber( -1 ),
	mRenderTexMaterialNumber( -1 ){
			mWorld.setIdentity();
			//データを移していく
			mVertexBuffer = data->mVertexBuffer;
			mIndexBuffer = NEW IndexBuffer[ mMaterialCount ];
			mMaterial = NEW Material[ mMaterialCount ];
			for( unsigned long i = 0; i < mMaterialCount; ++i ){
				mIndexBuffer[ i ] = data->mIndexBuffer[ i ];
				mMaterial[ i ].mAmbient = data->mMeshMaterial[ i ].mAmbient;
				mMaterial[ i ].mDiffuse = data->mMeshMaterial[ i ].mDiffuse;
				mMaterial[ i ].mSpecular = data->mMeshMaterial[ i ].mSpecular;
				ostringstream oss;
				oss.str( "" );
				oss << "Data/Model/" << data->mMeshMaterial[ i ].mTextureName;
				if(  -1 != oss.str().find( ".png" ) ){
					Graphics::Texture t = Graphics::Texture::create( oss.str().c_str() );
					mMaterial[ i ].mTexture = t;
				}
				mMaterial[ i ].mNumFace = data->mMeshMaterial[ i ].mNumFace;
			}
	}
	~Impl(){
		SAFE_DELETE_ARRAY( mIndexBuffer );
		SAFE_DELETE_ARRAY( mMaterial );
	}
	void setTexture( const Mashiro::Graphics::Texture& tex, int num ){
		mUseTexture = tex;
		mTexMaterialNumber = num;
	}
	void setTexture( const Mashiro::Graphics::RenderTarget& tex, int num ){
		mUseRenderTargetTexture = tex;
		mRenderTexMaterialNumber = num;
	}
	void setAngle( const Vector3& angle ){
		mAngle = angle;
	}
	void setPosition( const Vector3& position ){
		mPosition = position;
	}
	void setColor( const Vector3& color ){
		mColor = color;
	}
	void setPosition( const Vector2& in, const Matrix& proj, const Matrix& view ){
		Matrix convert;
		mPosition = convert.convertWorld( 
			Mashiro::WorkSpace::instance().width(),
			Mashiro::WorkSpace::instance().height(),
			&Vector3( in.x, in.y, 0.f ),
			proj, view );
	}
	void setScale( const Vector3& scale ){
		mScale = scale;
	}
	void setTransparaency( float c ){
		mTrans = c;
	}
	Matrix worldMatrix() {
		//今現在の状況のワールドマトリクスを返す
		Matrix trans, y, x, z, scale;
		//ワールド行列をセット
		trans.setTranslation( mPosition );
		y.setRotationY( mAngle.y );
		x.setRotationX( mAngle.x );
		z.setRotationZ( mAngle.z );
		scale.setScaling( mScale );
		mWorld.setIdentity();
		mWorld.setMul( mWorld, scale );
		mWorld.setMul( mWorld, z );
		mWorld.setMul( mWorld, x );
		mWorld.setMul( mWorld, y );
		mWorld.setMul( mWorld, trans );
		return mWorld;
	}
	void draw() {
		Graphics::Manager m = Graphics::Manager::instance();	
		//ワールド行列を作る
		Matrix trans, y, x, z, scale;
		//描画(マテリアルごとに分割しながら描画します
		for( unsigned long i = 0; i < mMaterialCount; ++i ){
			//Bufferをセット
			m.setVertexBuffer( mVertexBuffer );
			m.setIndexBuffer( mIndexBuffer[ i ] );
			//ワールド行列をセット
			trans.setTranslation( mPosition );
			y.setRotationY( mAngle.y );
			x.setRotationX( mAngle.x );
			z.setRotationZ( mAngle.z );
			scale.setScaling( mScale );
			mWorld.setIdentity();
			mWorld.setMul( mWorld, scale );
			mWorld.setMul( mWorld, z );
			mWorld.setMul( mWorld, x );
			mWorld.setMul( mWorld, y );
			mWorld.setMul( mWorld, trans );
			//GraphicsManagerに渡す
			m.setWorldMatrix( mWorld );
			m.setAmbient( Vector4( mMaterial[ i ].mAmbient ) );
			Vector3 dc;
			dc.setMul( Vector3( mMaterial[ i ].mDiffuse.x, mMaterial[ i ].mDiffuse.y, mMaterial[ i ].mDiffuse.z ), mColor );
			m.setDiffuse( Vector4( dc.x, dc.y, dc.z, mTrans  ) );
			m.setSpecular( mMaterial[ i ].mSpecular );
			//テクスチャorマテリアル切り替え
			if( mTexMaterialNumber == i ){
				m.setTexture( mUseTexture, 0 );
			} else {
				m.setTexture( mMaterial[ i ].mTexture, 0 );
			}
			if( mRenderTexMaterialNumber == i ){
				m.setTexture( mUseRenderTargetTexture, 0 );
			} else {
				//m.setTexture( mMaterial[ i ].mTexture, 0 );
			}
			m.drawIndexed( mIndexBuffer[ i ].indexNumber(), 0 );
		}
	}
	void draw( const Matrix& world ) {
		Graphics::Manager m = Graphics::Manager::instance();
		
		//描画(マテリアルごとに分割しながら描画します
		for( unsigned long i = 0; i < mMaterialCount; ++i ){
			//Bufferをセット
			m.setVertexBuffer( mVertexBuffer );
			m.setIndexBuffer( mIndexBuffer[ i ] );
			mWorld = world;
			m.setWorldMatrix( mWorld );
			m.setAmbient( Vector4( mMaterial[ i ].mAmbient ) );
			Vector3 dc;
			dc.setMul( Vector3( mMaterial[ i ].mDiffuse.x, mMaterial[ i ].mDiffuse.y, mMaterial[ i ].mDiffuse.z ), mColor );
			m.setDiffuse( Vector4( dc.x, dc.y, dc.z, mTrans ) );
			m.setSpecular( mMaterial[ i ].mSpecular );
			//テクスチャorマテリアル切り替え
			if( mTexMaterialNumber == i ){
				m.setTexture( mUseTexture, 0 );
			} else {
				m.setTexture( mMaterial[ i ].mTexture, 0 );
			}
			if( mRenderTexMaterialNumber == i ){
				m.setTexture( mUseRenderTargetTexture, 0 );
			} else {
				//m.setTexture( mMaterial[ i ].mTexture, 0 );
			}
			m.drawIndexed( mIndexBuffer[ i ].indexNumber(), 0 );
		}
	}
	//モデル情報
	Matrix mWorld;
	Vector3 mPosition;
	Vector3 mAngle;
	Vector3 mScale;
	Vector3 mColor;
	float mTrans;
	//頂点バッファ
	VertexBuffer mVertexBuffer;
	//インデックスバッファ
	IndexBuffer* mIndexBuffer;
	//マテリアル
	struct Material {
		Vector4 mDiffuse;
		Vector4 mAmbient;
		Vector4 mSpecular;
		Graphics::Texture mTexture;
		const char* mFileName;
		unsigned long mNumFace; //マテリアルポリゴン数
	};
	Material* mMaterial;
	unsigned long mMaterialCount;

	//外部から強制貼り付け
	Graphics::Texture mUseTexture;
	Graphics::RenderTarget mUseRenderTargetTexture;
	int mRenderTexMaterialNumber;
	int mTexMaterialNumber;
};

}
}


#endif
