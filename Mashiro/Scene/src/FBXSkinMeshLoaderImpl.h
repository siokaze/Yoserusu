#ifndef INCLUDE_MASHIRO_GRAPHICS_FBX_SKIN_MESH_LOADERIMPL_H_
#define INCLUDE_MASHIRO_GRAPHICS_FBX_SKIN_MESH_LOADERIMPL_H_

#define FBXSDK_new_API

#include <D3DX10math.h>

#include <fbxsdk.h>
#include "Mashiro/Mashiro.h"
#include "Mashiro/WorkSpace/WorkSpace.h"
#include "Mashiro/Base/Impl/ReferenceType.h"
#include "Mashiro/Graphics/IndexBuffer.h"
#include "Mashiro/Graphics/VertexBuffer.h"
#include "Mashiro/Graphics/Texture.h"
#include "Mashiro/Scene/FBXSkinMeshLoader.h"
#include "Mashiro/Math/Vector4.h"
#include "Mashiro/Math/Vector2.h"
#include "Mashiro/Math/Matrix.h"


#include <fstream>
#include <sstream>

using namespace std;
using namespace Mashiro::Math;
using namespace Mashiro::Graphics;

namespace Mashiro{

namespace Scene {

class FBXSkinMeshLoader::Impl : public Mashiro::ReferenceType {
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
	//インデックス構造体
	struct FBX_INDEX{
		unsigned long mIndex[ 5000 ];	//4
		int mCount;				//4
	};
	FBX_INDEX* mMRSIndex;
	//ボーン構造体
	struct FBX_BONE{
		D3DXMATRIX mBindPose; //初期座標		(4*4)*4=64
		D3DXMATRIX mNowPose; //現在のポーズ		(4*4)*4=64
		FBX_BONE()
		{
			D3DXMatrixIdentity( &mNowPose );
		}
	};
	FBX_BONE* mMRSBone;
	struct FBX_ANIMETION_FRAME{
		int mStartFrame;
		int mEndFrame;
	};
	FBX_ANIMETION_FRAME* mMRSAnimationFrame;
	struct FBX_FRAME_BONE{
		D3DXMATRIX mFramePose[ 27 ];					//4*4*4 = 64
	};
	FBX_FRAME_BONE* mMRSFrameBone;
	struct FBX_POLY_TABLE{
		int mPolyIndex[ 20 ]; //ポリゴン番号				4*20=80
		int mIndex123[ 20 ]; //3つの頂点のうち、何番目か	3*20=80
		int mNumRef; //属しているポリゴンの数					  4
		FBX_POLY_TABLE()
		{	
			ZeroMemory(this, sizeof(FBX_POLY_TABLE));
		}
	};
	FBX_POLY_TABLE* mMRSPolyTabel;
	//マテリアル情報
	struct MeshMaterial{
		Vector4 mAmbient;					//4*4=16
		Vector4 mDiffuse;					//4*4=16
		Vector4 mSpecular;					//4*4=16				
		unsigned long mNumFace;					 //4
		MeshMaterial()
		{
			ZeroMemory(this,sizeof(MeshMaterial));
		}
	};
	MeshMaterial* mMeshMaterial;
	unsigned long mFaceCount;
#pragma pack( pop )
public:
	Impl() :
	mFaceCount( 0 ),
	mVertexCount( 0 ), //頂点数
	mIndexCount( 0 ),
	mUVCount( 0 ),
	mMaterialCount( 0 ), //マテリアル数 = インデックスバッファ数
	mBoneCount( 0 ),
	mMeshMaterial( 0 ),
	mIndex( 0 ),
	mMRSAnimationFrame( 0 ),
	mMRSVertex( 0 ),
	mMRSIndex( 0 ),
	mMRSPolyTabel( 0 ),
	mMRSFrameBone( 0 ),
	mMRSBone( 0 ){
	}
	~Impl(){
		if( mMRSBone ){
			SAFE_DELETE_ARRAY( mMRSBone );
		}
		if( mCluster ){
			SAFE_DELETE_ARRAY( mCluster );
		}
		if( mMeshMaterial ){
			SAFE_DELETE_ARRAY( mMeshMaterial );
		}
		if( mIndex ){
			SAFE_DELETE_ARRAY( mIndex );
		}
		if(	mMRSVertex ){
			SAFE_DELETE_ARRAY( mMRSVertex );
		}
		if( mManager ){
			mManager->Destroy();
			mManager = 0;
		}
	}
	void createModel( const char* fileName ){
		//初期化
		if( bool hr = !initFBX( fileName ) ){
			STRONG_ASSERT( hr != false && "FBXローダ初期化失敗" );
		}
		//ルートノードの取得
		FbxNode* rootNode = mScene->GetRootNode();
		FbxMesh* mesh = 0;

		getRootNode( &rootNode, &mesh );

		//事前に頂点数、ポリコン数を調べる
		mVertexCount = mesh->GetControlPointsCount();
		mUVCount = mesh->GetTextureUVCount();
		mFaceCount = mesh->GetPolygonCount();

		//一時的なメモリ確保
		( mVertexCount < mUVCount ) ? mMRSVertex = NEW FBX_SKINMESH_VERTEX[ mUVCount ] : mMRSVertex = NEW FBX_SKINMESH_VERTEX[ mVertexCount ];
		int* faceBuffer = NEW int[ mFaceCount*3 ];

		//頂点読み込み
		loadVertex( mesh, mMRSVertex );

		//ポリコン情報
		loadIndex( mesh, faceBuffer );

		//法線読み込み
		loadNormal( mesh, faceBuffer, mMRSVertex );

		//カラーの読み込み
		loadColor( mesh, faceBuffer, mMRSVertex );

		//テクスチャ座標読み込み
		loadUv( mesh, mMRSVertex );

		//マテリアルの読み込み
		loadMaterial( mesh );

		//頂点からポリゴンを逆引きしたいので、逆引きテーブル
		mMRSPolyTabel = NEW FBX_POLY_TABLE[ mVertexCount ];
		for( unsigned long i = 0; i < mVertexCount; ++i ){
			for( unsigned long k = 0; k < mFaceCount; ++k ){
				for( int m = 0; m < 3; ++m ){
					if( mesh->GetPolygonVertex( k, m ) == i ){
						mMRSPolyTabel[ i ].mPolyIndex[ mMRSPolyTabel[ i ].mNumRef ] = k;
						mMRSPolyTabel[ i ].mIndex123[ mMRSPolyTabel[ i ].mNumRef ] = m;
						mMRSPolyTabel[ i ].mNumRef++;
					}
				}
			}
		}

		//スキン情報（ショイント、ウェイト）読み込み
		readSkinInfo( mesh, mMRSVertex, mMRSPolyTabel );
		
		//頂点バッファを生成

		if( mVertexCount < mUVCount ) {
			mVertexCount = mUVCount;
		}
		mVertex = VertexBuffer::create( mVertexCount, sizeof( SkinMeshVertex ) );

		SkinMeshVertex* v = NULL;
		if( mVertex.lock((void**)&v)){
			for( unsigned long i = 0; i < mVertexCount; i++ ){
				v[ i ].mPosition = mMRSVertex[ i ].mPosition;
				v[ i ].mNormal = mMRSVertex[ i ].mNormal;
				v[ i ].mUV = mMRSVertex[ i ].mUV;
				v[ i ].mBoneWeight = mMRSVertex[ i ].mBoneWeight;
				memcpy( v[ i ].mBoneIndex, mMRSVertex[ i ].mBoneIndex, sizeof( mMRSVertex[ i ].mBoneIndex ) );
			}

			//アニメーションフレームを取り出す
			frameMatrix( 0, 500 );

			mVertex.unlock();
		}

		SAFE_DELETE_ARRAY( mMRSPolyTabel );
		SAFE_DELETE_ARRAY( faceBuffer );
	}
	//初期化
	bool initFBX( const char* fileName ){
		mManager = FbxManager::Create();

		mImport = FbxImporter::Create( mManager, "importer" );

		mImport->Initialize( fileName );

		mScene = FbxScene::Create( mManager, "scene" );
		
		mImport->Import( mScene );

		return true;
	}
	//ルートノードを取得
	void getRootNode( FbxNode** root, FbxMesh** mesh ){
		FbxNodeAttribute *attr=(*root)->GetNodeAttribute();
		FbxNodeAttribute::EType type;

		//	親にデータが入っているケースは少ない
		if(attr!=NULL)
		{
			//	タイプを確認
			type =attr->GetAttributeType();

			//	メッシュだった場合
			if(type==FbxNodeAttribute::eMesh)
			{
				//	データ読み込み
				(*mesh) = (*root)->GetMesh();
			}
		}
		else
		{
			//	ぶら下がっている子ノード数を取得
			int iNumChild = (*root)->GetChildCount();

			//	ノード数分ループ
			for(int i=0;i<iNumChild;i++)
			{
				//	ノードを取得する
				FbxNode* pChildNode = (*root)->GetChild(i);

				//	ノードが存在しない!?
				if(!pChildNode) continue;

				//	取得したノードのタイプを確認
				attr = pChildNode->GetNodeAttribute();
				type = attr->GetAttributeType();

				//	ノードタイプがメッシュの場合
				if( type == FbxNodeAttribute::eMesh)
				{
					//	データ読み込み
					(*mesh) = pChildNode->GetMesh();
					break;
				}
			}		
		}
	}
	//インデックス読み込み
	void loadIndex( const FbxMesh* mesh, int* index ){
		//Fbxの頂点インデックス先頭のポインタを取得
		int* iIndex = mesh->GetPolygonVertices();
		//4*面数*3のインデックスコピー
		memcpy( index, iIndex, sizeof(int)*mFaceCount*3 );
	}
	//頂点の読み込み
	void loadVertex( FbxMesh* mesh, FBX_SKINMESH_VERTEX* vb ){
		int* piIndex=mesh->GetPolygonVertices();//	頂点インデックス読み込み

		//	頂点数ではなく、面から頂点データを取得する
		for(unsigned long i = 0; i < mFaceCount; i++)
		{		
			//	ポリゴンを構成するインデックスを取得
			int startIndex=mesh->GetPolygonVertexIndex(i);
		
			//	頂点番号を取得
			int idx0=piIndex[ startIndex ];		
			int idx1=piIndex[ startIndex+1 ];
			int idx2=piIndex[ startIndex+2 ];

			//	UV数のほうが多い場合はUVインデックスを基準にする
			if(mVertexCount<mUVCount)	
			{
				idx0 = mesh->GetTextureUVIndex(i,0,FbxLayerElement::eTextureDiffuse);
				idx1 = mesh->GetTextureUVIndex(i,1,FbxLayerElement::eTextureDiffuse);
				idx2 = mesh->GetTextureUVIndex(i,2,FbxLayerElement::eTextureDiffuse);
			}

			//	頂点データ0の取得
			int index=mesh->GetPolygonVertex(i,0);
			FbxVector4 Coord=mesh->GetControlPointAt(index);
			vb[idx0].mPosition.x =  static_cast<float>(Coord.mData[0]);
			vb[idx0].mPosition.y =  static_cast<float>(Coord.mData[1]);
			vb[idx0].mPosition.z =  static_cast<float>(Coord.mData[2]);

			//	頂点データ1の取得
			index=mesh->GetPolygonVertex(i,1);
			Coord=mesh->GetControlPointAt(index);
			vb[idx1].mPosition.x =  static_cast<float>(Coord.mData[0]);
			vb[idx1].mPosition.y =  static_cast<float>(Coord.mData[1]);
			vb[idx1].mPosition.z =  static_cast<float>(Coord.mData[2]);

			//	頂点データ2の取得
			index=mesh->GetPolygonVertex(i,2);
			Coord=mesh->GetControlPointAt(index);
			vb[idx2].mPosition.x =  static_cast<float>(Coord.mData[0]);
			vb[idx2].mPosition.y =  static_cast<float>(Coord.mData[1]);
			vb[idx2].mPosition.z =  static_cast<float>(Coord.mData[2]);

		}
	}
	//法線の読み込み
	void loadNormal( FbxMesh* mesh, const int* index, FBX_SKINMESH_VERTEX* vb ){
		FbxVector4 normal;

		for(unsigned long i = 0; i < mFaceCount; i++)
		{	
			//	ポリゴンを構成している頂点のインデックスバッファ
			//	開始位置を取得
			int startIdx=mesh->GetPolygonVertexIndex(i);
		
			//	開始位置を起点とし、三頂点分のインデックスを取得
			int idx0 = index[ startIdx ];
			int idx1 = index[ startIdx + 1 ];
			int idx2 = index[ startIdx + 2 ];

			if(idx0<=0) continue;
			//	UV数のほうが多い場合はUVインデックスを基準にする
			if(mVertexCount<mUVCount)	
			{
				idx0 = mesh->GetTextureUVIndex(i,0,FbxLayerElement::eTextureDiffuse);
				idx1 = mesh->GetTextureUVIndex(i,1,FbxLayerElement::eTextureDiffuse);
				idx2 = mesh->GetTextureUVIndex(i,2,FbxLayerElement::eTextureDiffuse);
			}

			//	ポリゴンを構成している0頂点の法線を取得
			mesh->GetPolygonVertexNormal(i,0,normal);

			//	取得した法線を頂点情報にコピーしていく。
			vb[idx0].mNormal.x = -static_cast<float>(normal[0]);
			vb[idx0].mNormal.y = static_cast<float>(normal[1]);
			vb[idx0].mNormal.z = static_cast<float>(normal[2]);

			//	ポリゴンを構成している1頂点の法線を取得
			mesh->GetPolygonVertexNormal(i,1,normal);
			vb[idx1].mNormal.x = -static_cast<float>(normal[0]);
			vb[idx1].mNormal.y = static_cast<float>(normal[1]);
			vb[idx1].mNormal.z = static_cast<float>(normal[2]);

			//	ポリゴンを構成している2頂点の法線を取得
			mesh->GetPolygonVertexNormal(i,2,normal);
			vb[idx2].mNormal.x = -static_cast<float>(normal[0]);
			vb[idx2].mNormal.y = static_cast<float>(normal[1]);
			vb[idx2].mNormal.z = static_cast<float>(normal[2]);
		}
	}
	//テクスチャの読み込み
	void loadUv( FbxMesh* mesh, FBX_SKINMESH_VERTEX* vb ){
		const FbxLayerElementUV* pUV=0;

		//	FBXテクスチャ座標の先頭ポインタを取得
		pUV = mesh->GetLayer(0)->GetUVs();

		//	UVの数分ループ
		for(unsigned long i = 0; i < mUVCount; i++)
		{
			FbxVector2 v2;

			//	テクスチャ座標取得
			v2=pUV->GetDirectArray().GetAt(i);

			//	テクスチャ座標を頂点配列にコピー
			vb[i].mUV.x = static_cast<float>(v2.mData[0]);
			vb[i].mUV.y = 1.0f - static_cast<float>(v2.mData[1]);
		}
	}
	//カラーの読み込み
	void loadColor( FbxMesh* mesh, const int* index, FBX_SKINMESH_VERTEX* vb ){
		const FbxLayerElementVertexColor* color=0;

		//	FBXテクスチャ座標の先頭ポインタを取得
		color = mesh->GetLayer(0)->GetVertexColors();

		//	UVの数分ループ
		if( color ){
			for(unsigned long i = 0; i < mUVCount; i++)
			{
				FbxColor c;

				//	テクスチャ座標取得
				c = color->GetDirectArray().GetAt( i );

				//	テクスチャ座標を頂点配列にコピー
				vb[i].mColor.x = static_cast<float>(c.mRed);
				vb[i].mColor.y = static_cast<float>(c.mGreen);
				vb[i].mColor.z = static_cast<float>(c.mBlue);
				vb[i].mColor.w = static_cast<float>(c.mAlpha);
			}
		} else {
			for(unsigned long i = 0; i < mUVCount; i++){
				vb[i].mColor.set( 1.f, 1.f, 1.f, 1.f );
			}
		}
	}
	//マテリアルの読み込み
	void loadMaterial( FbxMesh* mesh ){
		FbxNode* pNode	= mesh->GetNode();
		mNumMaterial	= pNode->GetMaterialCount();
		mMeshMaterial = NEW MeshMaterial[mNumMaterial];
		mMRSIndex = NEW FBX_INDEX[ mNumMaterial ];
		//	マテリアルの数だけインデックスバッファーを作成
		mIndex = NEW IndexBuffer[mNumMaterial];

		for(unsigned long i=0;i<mNumMaterial;i++)
		{
			//	フォンモデルを想定
			FbxSurfaceMaterial* pMaterial=pNode->GetMaterial(i);
			FbxSurfacePhong* pPhong=(FbxSurfacePhong*)pMaterial;


			//	アンビエントカラー(環境光)

			FbxDouble3 d3Ambient=pPhong->Ambient;
			mMeshMaterial[i].mAmbient.x = static_cast<float>(d3Ambient.mData[0]);
			mMeshMaterial[i].mAmbient.y = static_cast<float>(d3Ambient.mData[1]);
			mMeshMaterial[i].mAmbient.z = static_cast<float>(d3Ambient.mData[2]);
			mMeshMaterial[i].mAmbient.w = 1.0f;

			//	ディフューズカラー(拡散反射光)
			FbxDouble3 d3Diffuse=pPhong->Diffuse;
			mMeshMaterial[i].mDiffuse.x = static_cast<float>(d3Diffuse.mData[0]);
			mMeshMaterial[i].mDiffuse.y = static_cast<float>(d3Diffuse.mData[1]);
			mMeshMaterial[i].mDiffuse.z = static_cast<float>(d3Diffuse.mData[2]);
			mMeshMaterial[i].mDiffuse.w = 1.0f;
			
			//	スペキュラカラー(鏡面反射光)
			//FbxDouble3 d3Specular=pPhong->Specular;
			//mMaterial[i].mSpecular.x = static_cast<float>(d3Specular.mData[0]);
			//mMaterial[i].mSpecular.y = static_cast<float>(d3Specular.mData[1]);
			//mMaterial[i].mSpecular.z = static_cast<float>(d3Specular.mData[2]);
			//mMaterial[i].mSpecular.w = 1.0f;
			
			//テクスチャー（ディフューズテクスチャーのみ）
			FbxProperty lProperty;
			lProperty=pMaterial->FindProperty(FbxSurfaceMaterial::sDiffuse);
			FbxTexture* texture= lProperty.GetSrcObject<FbxTexture>();

			//	テクスチャの読み込み
			if(texture)
			{			
				//mMaterial[ i ].mFileName = texture->GetName();
				//mMaterial[i].mTexture = Texture::create( "robo3.png" );
			}	

			//マテリアルの数だけインデックスバッファーを作成
			int iCount=0;
			unsigned long* pIndex=NEW unsigned long[mFaceCount*3];//とりあえず、メッシュ内のポリゴン数でメモリ確保
	
			//そのマテリアルであるインデックス配列内の開始インデックスを調べる　さらにインデックスの個数も調べる		
			iCount=0;
			for(unsigned long k=0;k<mFaceCount;k++)
			{
				const FbxLayerElementMaterial* mat = mesh->GetLayer(0)->GetMaterials();
				int matId=mat->GetIndexArray().GetAt(k);

				if(matId==i)
				{
					//	頂点数よりUVが多い場合、UVインデックスを使用
					if(mVertexCount<mUVCount) 
					{
						pIndex[iCount]=mesh->GetTextureUVIndex(k,0,FbxLayerElement::eTextureDiffuse);
						pIndex[iCount+1]=mesh->GetTextureUVIndex(k,1,FbxLayerElement::eTextureDiffuse);
						pIndex[iCount+2]=mesh->GetTextureUVIndex(k,2,FbxLayerElement::eTextureDiffuse);
					}
					else
					{
						pIndex[iCount]=mesh->GetPolygonVertex(k,0);
						pIndex[iCount+1]=mesh->GetPolygonVertex(k,1);
						pIndex[iCount+2]=mesh->GetPolygonVertex(k,2);
					}		
				
					iCount+=3;
				}
			}

			mIndex[ i ] = Mashiro::Graphics::IndexBuffer::create( iCount );
			mMRSIndex[ i ].mCount = iCount;
			unsigned long* indices = mIndex[ i ].lock();
			for( int x = 0; x < iCount; x++ ){
				indices[ x ] = pIndex[ x ];
				mMRSIndex[ i ].mIndex[ x ] = pIndex[ x ];
			}
			mIndex[ i ].unlock( &indices );

			mMeshMaterial[ i ].mNumFace = iCount / 3;

			SAFE_DELETE_ARRAY( pIndex );
		}
	}

	//	現在の姿勢行列を取得する
	D3DXMATRIX getCurrentPoseMatrix(int index){
		D3DXMATRIX inv;
		D3DXMatrixInverse(&inv,0,&mMRSBone[index].mBindPose);	//	FBXのバインドポーズの逆行列を取得
		D3DXMATRIX ret=inv*mMRSBone[index].mNowPose;
		
		return ret;
	}

	//	新たな姿勢行列を取り出す。
	void setNewPoseMatrices(int frame){
		FbxTime time;
		//	フレームからアニメーション時間を取得
		time.SetTime(0,0,0,frame,0,FbxTime::eFrames30);

		//	ボーンの数分ループ実行
		for(unsigned long i=0;i<mBoneCount;i++){		
			D3DXMATRIX mat = mMRSFrameBone[ frame ].mFramePose[ i ];

			mMRSBone[i].mNowPose = mat;
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

	//	FBXファイルからボーン情報を取り出す
	void readSkinInfo(
		FbxMesh* mesh, 
		FBX_SKINMESH_VERTEX* vb,
		FBX_POLY_TABLE* polyTable ){
		
		//FBXから抽出すべき情報は、頂点ごとのボーンインデックス、頂点ごとのボーンウェイト、バインド行列、ポーズ行列　の4項目
	
		int i,k;
		int num = 0;//ボーン数

		//デフォーマーを得る 
		FbxDeformer* deformer = mesh->GetDeformer(0);
		FbxSkin* skinInfo = static_cast<FbxSkin*>(deformer);
	
		//	ボーンの数を取得
		num = skinInfo->GetClusterCount();
		mCluster = NEW FbxCluster*[ num ];		//	ボーンを生成

		//	ボーンの情報を取得する
		for( i=0;i<num;i++)
		{
			mCluster[i] = skinInfo->GetCluster(i);
		}

		//	通常の場合
		if(mVertexCount >= mUVCount)
		{
			//	それぞれのボーンに影響を受ける頂点を調べる
			for(i = 0; i < num; i++)
			{
				//	対象となるボーンに影響を受ける頂点の数を調べる
				int  numIndex = mCluster[i]->GetControlPointIndicesCount();

				//	ボーンに対応しているインデックスを取得
				int* index = mCluster[i]->GetControlPointIndices();

				//	ウェイト情報を取得
				double* pdWeight=mCluster[i]->GetControlPointWeights();

				//	頂点側からインデックスをたどって、頂点サイドで整理する
				for(k = 0; k < numIndex; k++)
				{
					//FBXやCGソフトがボーン4本以内とは限らない。5本以上の場合は、重みの大きい順に4本に絞る
					for(int m= 0; m < 4; m++) 
					{
						if( pdWeight[k] > vb[ index[k] ].mBoneWeight[m])
						{
							vb[ index[k] ].mBoneIndex[m] = i;
							vb[ index[k] ].mBoneWeight[m] = static_cast<float>(pdWeight[k]);				
							break;
						}
					}				
				}
			}
		}
		//UVインデックスベースの場合　（頂点数<UV数）
		else
		{
			int polyIndex=0;
			int UVIndex=0;

			//	それぞれのボーンに影響を受ける頂点を調べる
			for( i = 0; i < num ;i++)
			{
				//	対象となるボーンに影響を受ける頂点の数を調べる
				int numIndex = mCluster[i]->GetControlPointIndicesCount();

				//	対応している頂点のインデックスを取得
				int* index = mCluster[i]->GetControlPointIndices();

				//	それぞれの頂点に対する重みを取得
				double* pdWeight = mCluster[i]->GetControlPointWeights();

				//頂点側からインデックスをたどって、頂点サイドで整理する
				for(k = 0; k < numIndex; k++)
				{				
					//	その頂点を含んでいるポリゴンすべてに、ボーンとウェイトを適用する
					for(int p = 0; p < polyTable[ index[k] ].mNumRef; p++)
					{	
						//	頂点 → 属するポリゴン → そのポリゴンのUVインデックスと逆引き
						polyIndex = polyTable[ index[k] ].mPolyIndex[p];
						UVIndex = mesh->GetTextureUVIndex(polyIndex,polyTable[ index[k] ].mIndex123[p],FbxLayerElement::eTextureDiffuse);
								
						for(int m = 0; m < 4; m++) 
						{	
							if( pdWeight[k] > vb[ UVIndex ].mBoneWeight[m] )
							{
								vb[UVIndex].mBoneIndex[ m] = i;
								vb[UVIndex].mBoneWeight[m] = static_cast<float>(pdWeight[k]);
								break;	
							}		
						}
					}	
				}
			}
		}

		//
		//ボーンを生成
		mBoneCount = num;
		mMRSBone = NEW FBX_BONE[ num ];

		for( int j = 0; j < mBoneCount; j++)
		{
			FbxAMatrix mat;
			mCluster[j]->GetTransformLinkMatrix(mat);
		
			for(int x = 0; x < 4; x++)
			{
				for(int y = 0; y < 4; y++)
				{
					mMRSBone[j].mBindPose(y,x) = static_cast<float>(mat.Get(y,x));
					mMRSBone[j].mBindPose._11 *=-1;
				}
			}
		}
	}

	//アニメーションボーンを取り出す
	void frameMatrix( int frame1, int frame2 ){
		FbxTime time;
		if( mMRSFrameBone ){
			SAFE_DELETE_ARRAY( mMRSFrameBone );
			SAFE_DELETE( mMRSAnimationFrame );
		}
		mMRSFrameBone = NEW FBX_FRAME_BONE[ frame2 - frame1 ];
		mMRSAnimationFrame = NEW FBX_ANIMETION_FRAME;
		mMRSAnimationFrame->mStartFrame = frame1;
		mMRSAnimationFrame->mEndFrame = frame2;
		//	フレームからアニメーション時間を取得
		for( int i = frame1; i <  frame2; i++ ){
			time.SetTime(0,0,0,i,0,FbxTime::eFrames30);
			
			//	ボーンの数分ループ実行
			for(unsigned long j=0; j<mBoneCount; j++)
			{		
				D3DXMatrixIdentity( &mMRSFrameBone[ i ].mFramePose[ j ] );
				//	姿勢行列を取り出す
				FbxAMatrix mat=mCluster[ j ]->GetLink()->EvaluateGlobalTransform( time );

				//	DirectXの行列変数に変換を実行
				for(int x=0;x<4;x++)
				{
					for(int y=0;y<4;y++)
					{
						mMRSFrameBone[ i ].mFramePose[ j ]( y, x ) = static_cast< float >( mat.Get( y, x ) );
					}
				}
			}
		}
	}

	void createAnimationFile( const char* fileName, int frame1, int frame2 ){
		ostringstream oss;
		oss.str( "" );
		oss << fileName << ".sin";
		
		//まずはファイルを作成
		FILE* fp;
		fp = fopen( oss.str().c_str(), "wb" );

		//---------ボーン数------------//
		fwrite( &mBoneCount, sizeof( mBoneCount ), 1, fp );

		//---------BoneData----------//
		fwrite( mMRSBone, sizeof( FBX_BONE ), mBoneCount, fp );

		//---------フレームの時間-------------------------//
		fwrite( mMRSAnimationFrame, sizeof( FBX_ANIMETION_FRAME ), 1, fp );
		
		//---------フレームのアニメーションデータ-------//
		frameMatrix( frame1, frame2 );
		fwrite( mMRSFrameBone, sizeof( FBX_FRAME_BONE ), frame2-frame1, fp );

		fclose( fp );
	}

	void createFile( const char* fileName ){
		ostringstream oss;
		oss.str( "" );
		oss << fileName << ".mrs";
		
		//まずはファイルを作成
		FILE* fp;
		fp = fopen( oss.str().c_str(), "wb" );

		//---------頂点数----------------//
		fwrite( &mVertexCount, sizeof( mVertexCount ), 1, fp );

		//---------インデックス数-------//
		fwrite( &mIndexCount, sizeof( mIndexCount ), 1, fp );

		//---------UV数-----------------//
		fwrite( &mUVCount, sizeof( mUVCount ), 1, fp );

		//---------マテリアル数--------//
		mMaterialCount = mNumMaterial;
		fwrite( &mMaterialCount, sizeof( mMaterialCount ), 1, fp );

		//---------ボーン数------------//
		fwrite( &mBoneCount, sizeof( mBoneCount ), 1, fp );

		//---------VertexBuffer-------//
		int count = 0;
		( mVertexCount < mUVCount ) ? count = mUVCount : count = mVertexCount;
		fwrite( mMRSVertex, sizeof( FBX_SKINMESH_VERTEX ), count, fp );

		//---------IndexBuffer--------//
		fwrite( mMRSIndex, sizeof( FBX_INDEX ), mMaterialCount, fp );

		//---------Material-----------//
		fwrite( mMeshMaterial, sizeof( MeshMaterial ), mMaterialCount, fp );

		fclose( fp );
	}


	//Fbxファイル読み込み関連
	FbxManager* mManager;
	FbxImporter* mImport;
	FbxScene* mScene;

	//バッファ関連
	VertexBuffer mVertex;
	IndexBuffer* mIndex;

	//マテリアル関連
	unsigned long mNumMaterial;

	//アニメーション関連
	int mAnimFrame;

	//ボーン関連
	FbxCluster** mCluster;
	D3DXMATRIX mBoneData[ 255 ];
};

}
}

#endif