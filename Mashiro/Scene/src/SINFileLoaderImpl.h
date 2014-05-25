#ifndef INCLUDE_MASHIRO_GRAPHICS_SIN_FILELOADERIMPL_H_
#define INCLUDE_MASHIRO_GRAPHICS_SIN_FILELOADERIMPL_H_

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
#include "Mashiro/Scene/SINFileLoader.h"
#include "Mashiro/Math/Vector4.h"
#include "Mashiro/Math/Vector2.h"
using namespace std;
using namespace Mashiro::Math;
using namespace Mashiro::Graphics;

namespace Mashiro{

namespace Scene {

class SINFileLoader::Impl : public Mashiro::ReferenceType {
public:
#pragma pack( push, 1 )
	unsigned long mBoneCount;
	//	FBXスキンメッシュ用頂点構造体
	//ボーン構造体
	struct FBX_BONE{
		D3DXMATRIX mBindPose; //初期座標
		D3DXMATRIX mNowPose; //現在のポーズ
	};
	FBX_BONE* mSINBone;
	struct FBX_ANIMETION_FRAME{
		int mStartFrame;
		int mEndFrame;
	};
	FBX_ANIMETION_FRAME* mSINAnimationFrame;
	struct FBX_FRAME_BONE{
		D3DXMATRIX mFramePose[ 27 ];					//4*4*4 = 64
	};
	FBX_FRAME_BONE* mSINFrameBone;
#pragma pack( pop )
public:
	D3DXMATRIX mBoneData[ 255 ];
public:
	Impl( const char* fileName ) : 
	mBoneCount( 0 ),
	mSINFrameBone( 0 ),
	mSINBone( 0 ),
	mSINAnimationFrame( 0 ){
		createAnimation( fileName );
	}
	~Impl(){
		destory();
	}
	void destory(){
		if( mSINBone ){
			SAFE_DELETE_ARRAY( mSINBone );
		}
		if( mSINFrameBone ){
			SAFE_DELETE_ARRAY( mSINFrameBone );
		}
		if( mSINAnimationFrame ){
			SAFE_DELETE_ARRAY( mSINAnimationFrame );
		}
	}
	void createAnimation( const char* fileName ){
		//一回NEWしたものを破棄したい
		destory();
		FILE* fp;
		fp = fopen( fileName, "rb" );
		if( fp != 0 ){
			//---------ボーン数-----------//
			fread( &mBoneCount, sizeof( mBoneCount ), 1, fp );

			//---------BoneData----------//
			mSINBone = NEW FBX_BONE[ mBoneCount ];
			fread( mSINBone, 128, mBoneCount, fp );

			//---------フレームの時間-------------------------//
			mSINAnimationFrame = NEW FBX_ANIMETION_FRAME();
			fread( mSINAnimationFrame, sizeof( FBX_ANIMETION_FRAME ), 1, fp );

			//-----アニメーションデータ------//
			int frame = mSINAnimationFrame->mEndFrame - mSINAnimationFrame->mStartFrame;
			mSINFrameBone = NEW FBX_FRAME_BONE[ frame ]; 
			fread( mSINFrameBone, sizeof( FBX_FRAME_BONE ), frame, fp );

			fclose( fp );
		} else {
			STRONG_ASSERT( NULL && "fopen ERR" );
		}
	}

	//	現在の姿勢行列を取得する
	D3DXMATRIX getCurrentPoseMatrix(int index){
		D3DXMATRIX inv;
		D3DXMatrixInverse(&inv,0,&mSINBone[index].mBindPose);	//	FBXのバインドポーズの逆行列を取得
		D3DXMATRIX ret=inv*mSINBone[index].mNowPose;
		
		return ret;
	}

	//	新たな姿勢行列を取り出す。
	void setNewPoseMatrices(int frame){
		//	ボーンの数分ループ実行
		for(unsigned long i=0;i<mBoneCount;i++){		
			D3DXMATRIX mat = mSINFrameBone[ frame ].mFramePose[ i ];

			mSINBone[i].mNowPose = mat;
		}

		//	シェーダーに渡すためのメンバ変数にコピー
		for( unsigned long j = 0; j < mBoneCount; j++ )
		{
			//	行列を取得
			D3DXMATRIX mat = getCurrentPoseMatrix(j);

			//	シェーダーに渡すので転置行列とする
			D3DXMatrixTranspose(&mat,&mat);

			//	行列をコピー
			mBoneData[j]=mat;
		}
	}
};

}
}

#endif