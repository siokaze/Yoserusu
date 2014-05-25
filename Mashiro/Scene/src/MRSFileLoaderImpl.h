#ifndef INCLUDE_MASHIRO_GRAPHICS_MSR_FILELOADERIMPL_H_
#define INCLUDE_MASHIRO_GRAPHICS_MSR_FILELOADERIMPL_H_

#include <D3DX10math.h>

#include <stdio.h>
#include <iostream>
#include <vector>
#include "Mashiro/Mashiro.h"
#include "Mashiro/Base/Impl/ReferenceType.h"
#include "Mashiro/Graphics/Vertex.h"
#include "Mashiro/Graphics/IndexBuffer.h"
#include "Mashiro/Graphics/VertexBuffer.h"
#include "Mashiro/Graphics/Texture.h"
#include "Mashiro/Scene/MRSFileLoader.h"
#include "Mashiro/Math/Vector4.h"
#include "Mashiro/Math/Vector2.h"
using namespace std;
using namespace Mashiro::Math;
using namespace Mashiro::Graphics;

namespace Mashiro{

namespace Scene {

class MRSFileLoader::Impl : public Mashiro::ReferenceType {
public:
#pragma pack( push, 1 )
	unsigned long mVertexCount; //頂点数
	unsigned long mIndexCount;
	unsigned long mUVCount;
	unsigned long mMaterialCount; //マテリアル数 = インデックスバッファ数
	unsigned long mBoneCount;
	//	FBXスキンメッシュ用頂点構造体
	struct FBX_SKINMESH_VERTEX
	{
		Vector3 mPosition;		//	ポジション            4*3=12
		Vector3 mNormal;		//	法線                  4*3=12
		Vector2 mUV;			//	テクスチャ座標        4*2=8
		Vector4 mColor;         //  カラー値              4*4=16
		Vector4 mBoneWeight;	//	ボーンの重み          4*4=16
		unsigned int mBoneIndex[4];		//	ボーンの番号  4*4=16
	};
	FBX_SKINMESH_VERTEX* mMRSVertex;
	//	FBXスキンメッシュ用頂点構造体
	struct FBX_VERTEX
	{
		Vector3 mPosition;		//	ポジション            4*3=12
		Vector3 mNormal;		//	法線                  4*3=12
		Vector2 mUV;			//	テクスチャ座標        4*2=8
		Vector4 mColor;         //  カラー値              4*4=16
	};
	FBX_VERTEX* mMRSVertex_Static;
	//インデックス構造体
	struct FBX_INDEX{
		unsigned long mIndex[ 5000 ];	//4
		int mCount;				//4
	};
	FBX_INDEX* mMRSIndex;
	//マテリアル情報
	struct MeshMaterial{
		Vector4 mAmbient;					//4*4=16
		Vector4 mDiffuse;					//4*4=16
		Vector4 mSpecular;					//4*4=16	
		char mTextureName[ 20 ];
		unsigned long mNumFace;					 //4
		MeshMaterial()
		{
			ZeroMemory(this,sizeof(MeshMaterial));
		}
	};
	MeshMaterial* mMeshMaterial;
#pragma pack( pop )
public:
	IndexBuffer* mIndexBuffer;
	VertexBuffer mVertexBuffer;
	D3DXMATRIX mBoneData[ 255 ];
public:
	Impl() : 
	mVertexCount( 0 ), //頂点数
	mIndexCount( 0 ),
	mUVCount( 0 ),
	mMaterialCount( 0 ), //マテリアル数 = インデックスバッファ数
	mMeshMaterial( 0 ),
	mIndexBuffer ( 0 ),
	mMRSVertex( 0 ),
	mMRSVertex_Static( 0 ),
	mMRSIndex( 0 ){
	}
	~Impl(){
		destory();
	}
	void destory(){
		if( mMRSIndex ){
			SAFE_DELETE_ARRAY( mMRSIndex );
		}
		if( mMRSVertex ){
			SAFE_DELETE_ARRAY( mMRSVertex );
		}
		if( mIndexBuffer ){
			SAFE_DELETE_ARRAY( mIndexBuffer );
		}
		if( mMeshMaterial ){
			SAFE_DELETE_ARRAY( mMeshMaterial );
		}
		if( mMRSVertex_Static ){
			SAFE_DELETE_ARRAY( mMRSVertex_Static );
		}
	}
	void createModel( const char* fileName ){
		//一回NEWしたものを破棄したい
		destory();
		FILE* fp;
		fp = fopen( fileName, "rb" );
		if( fp != 0 ){
			//---------頂点数----------------//
			fread( &mVertexCount, sizeof( mVertexCount ), 1, fp );

			//---------インデックス数-------//
			fread( &mIndexCount, sizeof( mIndexCount ), 1, fp );

			//---------UV数-----------------//
			fread( &mUVCount, sizeof( mUVCount ), 1, fp );

			//---------マテリアル数--------//
			fread( &mMaterialCount, sizeof( mMaterialCount ), 1, fp );

			//---------ボーン数------------//
			fread( &mBoneCount, sizeof( mBoneCount ), 1, fp );

			//---------VertexBuffer-------//
			( mVertexCount < mUVCount ) ? mMRSVertex = NEW FBX_SKINMESH_VERTEX[ mUVCount ] : mMRSVertex = NEW FBX_SKINMESH_VERTEX[ mVertexCount ]; 
			fread( mMRSVertex, 80, mVertexCount, fp );

			//---------IndexBuffer--------//
			mMRSIndex = NEW FBX_INDEX[ mMaterialCount ];
			fread( mMRSIndex, sizeof( FBX_INDEX ), mMaterialCount, fp );

			//---------Material-----------//
			mMeshMaterial = NEW MeshMaterial[ mMaterialCount ];
			fread( mMeshMaterial, sizeof( MeshMaterial ), mMaterialCount, fp );

			fclose( fp );
			//頂点ロード
			loadVertex();
			//インデックスロード
			loadIndex();
		} else {
			STRONG_ASSERT( NULL && "fopen ERR" );
		}
	}

	void static_createModel( const char* model ){
		//一回NEWしたものを破棄したい
		destory();
		FILE* fp;
		fp = fopen( model, "rb" );
		if( fp != 0 ){
			//---------頂点数----------------//
			fread( &mVertexCount, sizeof( mVertexCount ), 1, fp );

			//---------インデックス数-------//
			fread( &mIndexCount, sizeof( mIndexCount ), 1, fp );

			//---------UV数-----------------//
			fread( &mUVCount, sizeof( mUVCount ), 1, fp );

			//---------マテリアル数--------//
			fread( &mMaterialCount, sizeof( mMaterialCount ), 1, fp );

			//---------VertexBuffer-------//
			( mVertexCount < mUVCount ) ? mMRSVertex_Static = NEW FBX_VERTEX[ mUVCount ] : mMRSVertex_Static = NEW FBX_VERTEX[ mVertexCount ]; 
			fread( mMRSVertex_Static, sizeof(FBX_VERTEX), mVertexCount, fp );

			//---------IndexBuffer--------//
			mMRSIndex = NEW FBX_INDEX[ mMaterialCount ];
			fread( mMRSIndex, sizeof( FBX_INDEX ), mMaterialCount, fp );

			//---------Material-----------//
			mMeshMaterial = NEW MeshMaterial[ mMaterialCount ];
			fread( mMeshMaterial, sizeof( MeshMaterial ), mMaterialCount, fp );

			fclose( fp );
			//頂点ロード
			static_loadVertex();
			//インデックスロード
			loadIndex();
			//テクスチャロード
		} else {
			STRONG_ASSERT( NULL && "fopen ERR" );
		}
	}

	void static_loadVertex(){
		mVertexBuffer = VertexBuffer::create( mVertexCount, sizeof( Vertex ) );
		Vertex* v = NULL;
		if( mVertexBuffer.lock((void**)&v) ){
			for( unsigned long i = 0; i < mVertexCount; ++i ){
				v[ i ].mPosition = mMRSVertex_Static[ i ].mPosition;
				v[ i ].mNormal = mMRSVertex_Static[ i ].mNormal;
				v[ i ].mUV = mMRSVertex_Static[ i ].mUV;
				v[ i ].mColor = mMRSVertex_Static[ i ].mColor;
			}
			mVertexBuffer.unlock();
		}
	}

	void loadVertex(){
		mVertexBuffer = VertexBuffer::create( mVertexCount, sizeof( SkinMeshVertex ) );
		SkinMeshVertex* v = NULL;
		if( mVertexBuffer.lock( (void**)&v ) ){
			for( unsigned long i = 0; i < mVertexCount; ++i ){
				v[ i ].mPosition = mMRSVertex[ i ].mPosition;
				v[ i ].mNormal = mMRSVertex[ i ].mNormal;
				v[ i ].mUV = mMRSVertex[ i ].mUV;
				v[ i ].mBoneWeight = mMRSVertex[ i ].mBoneWeight;
				for( int j = 0; j < 4; ++j ){
					v[ i ].mBoneIndex[ j ] = mMRSVertex[ i ].mBoneIndex[ j ];
				}
			}
			mVertexBuffer.unlock();
		}
	}
	void loadIndex(){
		mIndexBuffer = NEW IndexBuffer[ mMaterialCount ];
		//マテリアルごとに分割していく
		unsigned long iCount = 0;
		unsigned long eCount = 0;
		unsigned long indexCount = 0;
		unsigned long* index = NULL;
		for( unsigned long i = 0; i < mMaterialCount; ++i ){
			mIndexBuffer[ i ] = Graphics::IndexBuffer::create( mMRSIndex[ i ].mCount );
			index = mIndexBuffer[ i ].lock();

			for( unsigned long j = 0; j < mMRSIndex[ i ].mCount; ++j ){
				index[ j ] = mMRSIndex[ i ].mIndex[ j ];
			}
			mIndexBuffer[ i ].unlock( &index );
		}
	}
};

}
}

#endif