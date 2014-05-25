#ifndef INCLUDE_MASHIRO_GRAPHICS_FBX_FILELOADERIMPL_H_
#define INCLUDE_MASHIRO_GRAPHICS_FBX_FILELOADERIMPL_H_

#define FBXSDK_new_API

#include <fbxsdk.h>
#include "Mashiro/Mashiro.h"
#include "Mashiro/Base/Impl/ReferenceType.h"
#include "Mashiro/Graphics/IndexBuffer.h"
#include "Mashiro/Graphics/VertexBuffer.h"
#include "Mashiro/Graphics/Texture.h"
#include "Mashiro/Scene/FBXFileLoader.h"
#include "Mashiro/Graphics/Vertex.h"
#include "Mashiro/Math/Vector4.h"
#include "Mashiro/Math/Vector2.h"
using namespace std;
using namespace Mashiro::Math;
using namespace Mashiro::Graphics;

namespace Mashiro{

namespace Scene {

class FBXFileLoader::Impl : public Mashiro::ReferenceType {
public:
	//頂点情報
	struct FBX_VERTEX{
		FBX_VERTEX() : mPosition( 0.f ), mNormal( 0.f ), mUv( 0.f ){ }
		Vector3 mPosition;
		Vector3 mNormal;
		Vector2 mUv;
		Vector4 mColor;
	};
	//マテリアル情報
	struct MeshMaterial{
		MeshMaterial() : mAmbient( 0.f ), mDiffuse( 0.f ), mSpecular( 0.f ){ }
		Vector4 mAmbient;
		Vector4 mDiffuse;
		Vector4 mSpecular;
		Graphics::Texture mTexture;
		const char* mFileName;
		unsigned long mNumFace; //マテリアルを使用しているポリゴン数
	};
public:
	Impl(){
	}
	~Impl(){
		if( mMaterial ){
			SAFE_DELETE_ARRAY( mMaterial );
		}
		if( mIndex ){
			SAFE_DELETE_ARRAY( mIndex );
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
		mNumVert = mesh->GetControlPointsCount();
		mNumUv = mesh->GetTextureUVCount();
		mNumFace = mesh->GetPolygonCount();

		//一時的なメモリ確保
		FBX_VERTEX* vb = NULL;
		( mNumVert < mNumUv ) ? vb = NEW FBX_VERTEX[ mNumUv ] : vb = NEW FBX_VERTEX[ mNumVert ];
		int* faceBuffer = NEW int[ mNumFace*3 ];

		//頂点読み込み
		loadVertex( mesh, vb );

		//ポリコン情報
		loadIndex( mesh, faceBuffer );

		//法線読み込み
		loadNormal( mesh, faceBuffer, vb );

		//カラーの読み込み
		loadColor( mesh, faceBuffer, vb );

		//テクスチャ座標読み込み
		loadUv( mesh, vb );

		//マテリアルの読み込み
		loadMaterial( mesh );
		
		//頂点バッファを生成

		if( mNumVert < mNumUv ) {
			mNumVert = mNumUv;
		}
		mVertex = VertexBuffer::create( mNumVert, sizeof( Vertex ) );
		Vertex* v = NULL;
		if( mVertex.lock((void**)&v)){
			for( unsigned long i = 0; i < mNumVert; i++ ){
				v[ i ].mPosition = vb[ i ].mPosition;
				v[ i ].mNormal = vb[ i ].mNormal;
				v[ i ].mUV = vb[ i ].mUv;
				v[ i ].mColor = vb[ i ].mColor;
			}
			mVertex.unlock();
		}

		SAFE_DELETE_ARRAY( faceBuffer );
		SAFE_DELETE_ARRAY( vb );
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
		memcpy( index, iIndex, sizeof(int)*mNumFace*3 );
	}
	//頂点の読み込み
	void loadVertex( FbxMesh* mesh, FBX_VERTEX* vb ){
		FbxVector4* Coord=mesh->GetControlPoints();

		int* piIndex=mesh->GetPolygonVertices();//	頂点インデックス読み込み

		//	頂点数ではなく、面から頂点データを取得する
		for(unsigned long i = 0; i < mNumFace; i++)
		{		
			//	ポリゴンを構成するインデックスを取得
			int startIndex=mesh->GetPolygonVertexIndex(i);
		
			//	頂点番号を取得
			int idx0=piIndex[ startIndex ];		
			int idx1=piIndex[ startIndex+1 ];
			int idx2=piIndex[ startIndex+2 ];

			//	UV数のほうが多い場合はUVインデックスを基準にする
			if(mNumVert<mNumUv)	
			{
				idx0 = mesh->GetTextureUVIndex(i,0,FbxLayerElement::eTextureDiffuse);
				idx1 = mesh->GetTextureUVIndex(i,1,FbxLayerElement::eTextureDiffuse);
				idx2 = mesh->GetTextureUVIndex(i,2,FbxLayerElement::eTextureDiffuse);
			}

			//	頂点データ0の取得
			int index=mesh->GetPolygonVertex(i,0);
			vb[idx0].mPosition.x =  static_cast<float>(Coord[ index ].mData[0]);
			vb[idx0].mPosition.y =  static_cast<float>(Coord[ index ].mData[1]);
			vb[idx0].mPosition.z =  static_cast<float>(Coord[ index ].mData[2]);

			//	頂点データ1の取得
			index=mesh->GetPolygonVertex(i,1);
			vb[idx1].mPosition.x =  static_cast<float>(Coord[ index ].mData[0]);
			vb[idx1].mPosition.y =  static_cast<float>(Coord[ index ].mData[1]);
			vb[idx1].mPosition.z =  static_cast<float>(Coord[ index ].mData[2]);

			//	頂点データ2の取得
			index=mesh->GetPolygonVertex(i,2);
			vb[idx2].mPosition.x =  static_cast<float>(Coord[ index ].mData[0]);
			vb[idx2].mPosition.y =  static_cast<float>(Coord[ index ].mData[1]);
			vb[idx2].mPosition.z =  static_cast<float>(Coord[ index ].mData[2]);

		}
	}
	//法線の読み込み
	void loadNormal( FbxMesh* mesh, const int* index, FBX_VERTEX* vb ){
		FbxVector4 normal;

		for(unsigned long i = 0; i < mNumFace; i++)
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
			if(mNumVert<mNumUv)	
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
	void loadUv( FbxMesh* mesh, FBX_VERTEX* vb ){
		const FbxLayerElementUV* pUV=0;

		//	FBXテクスチャ座標の先頭ポインタを取得
		pUV = mesh->GetLayer(0)->GetUVs();

		//	UVの数分ループ
		for(unsigned long i = 0; i < mNumUv; i++)
		{
			FbxVector2 v2;

			//	テクスチャ座標取得
			v2=pUV->GetDirectArray().GetAt(i);

			//	テクスチャ座標を頂点配列にコピー
			vb[i].mUv.x = static_cast<float>(v2.mData[0]);
			vb[i].mUv.y = 1.0f - static_cast<float>(v2.mData[1]);
		}
	}
	//カラーの読み込み
	void loadColor( FbxMesh* mesh, const int* index, FBX_VERTEX* vb ){
		const FbxLayerElementVertexColor* color=0;

		//	FBXテクスチャ座標の先頭ポインタを取得
		color = mesh->GetLayer(0)->GetVertexColors();

		//	UVの数分ループ
		if( color ){
			for(unsigned long i = 0; i < mNumUv; i++)
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
			for(unsigned long i = 0; i < mNumUv; i++){
				vb[i].mColor.set( 1.f, 1.f, 1.f, 1.f );
			}
		}
	}
	//マテリアルの読み込み
	void loadMaterial( FbxMesh* mesh ){
		FbxNode* pNode	= mesh->GetNode();
		mNumMaterial	= pNode->GetMaterialCount();
		mMaterial = NEW MeshMaterial[mNumMaterial];

		//	マテリアルの数だけインデックスバッファーを作成
		mIndex = NEW IndexBuffer[mNumMaterial];

		for(unsigned long i=0;i<mNumMaterial;i++)
		{
			//	フォンモデルを想定
			FbxSurfaceMaterial* pMaterial=pNode->GetMaterial(i);
			FbxSurfacePhong* pPhong=(FbxSurfacePhong*)pMaterial;


			//	アンビエントカラー(環境光)

			FbxDouble3 d3Ambient=pPhong->Ambient;
			mMaterial[i].mAmbient.x = static_cast<float>(d3Ambient.mData[0]);
			mMaterial[i].mAmbient.y = static_cast<float>(d3Ambient.mData[1]);
			mMaterial[i].mAmbient.z = static_cast<float>(d3Ambient.mData[2]);
			mMaterial[i].mAmbient.w = 1.0f;

			//	ディフューズカラー(拡散反射光)
			FbxDouble3 d3Diffuse=pPhong->Diffuse;
			mMaterial[i].mDiffuse.x = static_cast<float>(d3Diffuse.mData[0]);
			mMaterial[i].mDiffuse.y = static_cast<float>(d3Diffuse.mData[1]);
			mMaterial[i].mDiffuse.z = static_cast<float>(d3Diffuse.mData[2]);
			mMaterial[i].mDiffuse.w = 1.0f;
			
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
				mMaterial[ i ].mFileName = texture->GetName();
				mMaterial[i].mTexture = Graphics::Texture::create( "kazan3.png" );
			}	

			//マテリアルの数だけインデックスバッファーを作成
			int iCount=0;
			unsigned long* pIndex=NEW unsigned long[mNumFace*3];//とりあえず、メッシュ内のポリゴン数でメモリ確保
	
			//そのマテリアルであるインデックス配列内の開始インデックスを調べる　さらにインデックスの個数も調べる		
			iCount=0;
			for(unsigned long k=0;k<mNumFace;k++)
			{
				const FbxLayerElementMaterial* mat = mesh->GetLayer(0)->GetMaterials();
				int matId=mat->GetIndexArray().GetAt(k);

				if(matId==i)
				{
					//	頂点数よりUVが多い場合、UVインデックスを使用
					if(mNumVert<mNumUv) 
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
			unsigned long* indices = mIndex[ i ].lock();
			for( int x = 0; x < iCount; x++ ){
				indices[ x ] = pIndex[ x ];
			}
			mIndex[ i ].unlock( &indices );

			mMaterial[ i ].mNumFace = iCount / 3;

			SAFE_DELETE_ARRAY( pIndex );
		}
	}
	//Fbxファイル読み込み関連
	FbxManager* mManager;
	FbxImporter* mImport;
	FbxScene* mScene;

	//メッシュ関連
	unsigned long mNumVert;
	unsigned long mNumFace;
	unsigned long mNumUv;

	//バッファ関連
	VertexBuffer mVertex;
	IndexBuffer* mIndex;

	//マテリアル関連
	MeshMaterial* mMaterial;
	unsigned long mNumMaterial;
};

}
}

#endif