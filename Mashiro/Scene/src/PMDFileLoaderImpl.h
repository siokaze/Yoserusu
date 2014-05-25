#ifndef INCLUDE_MASHIRO_GRAPHICS_PMD_FILELOADERIMPL_H_
#define INCLUDE_MASHIRO_GRAPHICS_PMD_FILELOADERIMPL_H_

#include <stdio.h>
#include <iostream>
#include <vector>
#include "Mashiro/Mashiro.h"
#include "Mashiro/Base/Impl/ReferenceType.h"
#include "Mashiro/Graphics/Vertex.h"
#include "Mashiro/Graphics/IndexBuffer.h"
#include "Mashiro/Graphics/VertexBuffer.h"
#include "Mashiro/Graphics/Texture.h"
#include "Mashiro/Scene/PMDFileLoader.h"
#include "Mashiro/Math/Vector4.h"
#include "Mashiro/Math/Vector2.h"
using namespace std;
using namespace Mashiro::Math;
using namespace Mashiro::Graphics;

namespace Mashiro{

namespace Scene {

class PMDFileLoader::Impl : public Mashiro::ReferenceType {
public:
#pragma pack( push, 1 )
	//ヘッダー
	struct t_header{
		unsigned char magic[ 3 ];
		float version;
		unsigned char model_name[20];
		unsigned char comment[ 256 ]; 
	};
	t_header mHeader;
	//頂点リスト
	unsigned long vert_count;
	struct t_vertex{
		float pos[ 3 ];
		float normal_vec[ 3 ];
		float uv[ 2 ];
		unsigned short bone_num[ 2 ];
		unsigned char bone_weight;
		unsigned char edge_flag;
	};
	t_vertex* vertex;
	unsigned long material_count;
	struct t_material{
		float diffuse_color[3];
		float alpha;
		float specularity;
		float specular_color[3];
		float mirror_color[3];
		unsigned char toon_index;
		unsigned char edge_flag;
		unsigned long face_vert_count;
		char texture_file_name[20];
	};
	t_material *material;
	//面頂点リスト
	unsigned long face_vert_count;
	unsigned short* face_vert_index;
	static const int mFile_Max = 21; 
	//テクスチャーファイルの階層
	string mFileTexture;
#pragma pack( pop )
public:
	IndexBuffer* mIndexBuffer;
	VertexBuffer mVertexBuffer;
	//マテリアル
	struct Material{
		Vector4 mDiffuse;
		Vector4 mAmbient;
		Vector4 mSpecular;
		Graphics::Texture mTexture;
		const char* mFileName;
		unsigned long mNumFace; //マテリアルポリゴン数
	};
	Material* mMaterial;
public:
	Impl() : 
	vertex( 0 ),
	vert_count( 0 ),
	material( 0 ),
	face_vert_count( 0 ),
	face_vert_index( 0 ),
	mMaterial( 0 ),
	mIndexBuffer ( 0 ){
	}
	~Impl(){
		destory();
	}
	void destory(){
		if( vertex ){
			SAFE_DELETE_ARRAY( vertex );
		}
		if( face_vert_index ){
			SAFE_DELETE_ARRAY( face_vert_index );
		}
		if( material ){
			SAFE_DELETE_ARRAY( material );
		}
		if( mMaterial ){
			SAFE_DELETE_ARRAY( mMaterial );
		}
		if( mIndexBuffer ){
			SAFE_DELETE_ARRAY( mIndexBuffer );
		}
	}
	void createModel( const char* fileName ){
		//一回NEWしたものを破棄したい
		destory();
		FILE* fp;
		fopen_s( &fp, fileName, "rb" );
		if( fp != 0 ){
			//ヘッダー読み込み
			fread( &mHeader, sizeof( struct t_header ), 1, fp );

			//頂点読み込み
			fread( &vert_count, sizeof( vert_count ), 1, fp );

			//頂点データ読み込み
			vertex = NEW t_vertex[ vert_count ];
			fread( vertex, 38, vert_count, fp );

			//面頂点リスト
			fread( &face_vert_count, sizeof( face_vert_count ), 1, fp );

			//面頂点リストでーた読み込み
			face_vert_index = NEW unsigned short[ face_vert_count ];
			fread( face_vert_index, 2, face_vert_count, fp );

			//材質リスト材質数
			fread(&material_count, sizeof(material_count), 1, fp);

			//材質リストデータ読み込み
			material = NEW t_material[material_count];
			fread(material, 70, material_count, fp);

			fclose( fp );
			//先にマテリアルメモリ確保
			mMaterial = NEW Material[ material_count ];
			//マテリアルロード
			loadMaterial( fileName );
			//頂点ロード
			loadVertex();
			//インデックスロード
			loadIndex();
		} else {
			STRONG_ASSERT( NULL && "fopen ERR" );
		}
	}

	void loadVertex(){
		mVertexBuffer = VertexBuffer::create( vert_count, sizeof( Vertex ) );
		Vertex* v = NULL; 
		if( mVertexBuffer.lock( (void**)&v ) ){
			for( unsigned long i = 0; i < vert_count; ++i ){
				v[ i ].mPosition.set( vertex[ i ].pos[ 0 ], vertex[ i ].pos[ 1 ], vertex[ i ].pos[ 2 ] );
				v[ i ].mNormal.set( vertex[ i ].normal_vec[ 0 ], vertex[ i ].normal_vec[ 1 ], vertex[ i ].normal_vec[ 2 ] );
				v[ i ].mUV.set( vertex[ i ].uv[ 0 ], vertex[ i ].uv[ 1 ] );
				v[ i ].mColor.set( 1.f, 1.f, 1.f, 1.f );
			}
			mVertexBuffer.unlock();
		}
	}
	void loadIndex(){
		mIndexBuffer = NEW IndexBuffer[ material_count ];
		//マテリアルごとに分割していく
		unsigned long iCount = 0;
		unsigned long eCount = 0;
		unsigned long indexCount = 0;
		unsigned long* index = NULL;
		for( unsigned long i = 0; i < material_count; ++i ){
			unsigned long start = material[ i ].face_vert_count;
			eCount = eCount + start;
			mIndexBuffer[ i ] = IndexBuffer::create( start );
			index = mIndexBuffer[ i ].lock();
			indexCount = 0;
			for( unsigned long j = iCount; j < eCount; ){
				int pos = face_vert_index[ j ];
				index[ indexCount ] = face_vert_index[ j ];
				++indexCount;
				++j;
			}
			iCount += start;
			mMaterial[ i ].mNumFace =  start;
			mIndexBuffer[ i ].unlock( &index );
		}
	}
	void loadMaterial( const char* fileName ){
		for( unsigned long i = 0; i < material_count; ++i ){
			//マテリアルデータ移し
			mMaterial[ i ].mDiffuse.set(
				material[ i ].diffuse_color[ 0 ],
				material[ i ].diffuse_color[ 1 ],
				material[ i ].diffuse_color[ 2 ],
				material[ i ].alpha );
			mMaterial[ i ].mAmbient.set(
				material[ i ].mirror_color[ 0 ],
				material[ i ].mirror_color[ 1 ],
				material[ i ].mirror_color[ 2 ],
				1.f );
			mMaterial[ i ].mSpecular.set(
				material[ i ].specular_color[ 0 ],
				material[ i ].specular_color[ 1 ],
				material[ i ].specular_color[ 2 ],
				1.f );
			//テクスチャロード
			mFileTexture = "";
			string str( fileName );
			int p = str.rfind("/");//最後にこれが出たところ
			int t = 0;
			int sph = 0;
			int spa = 0;
			int tga = 0;
			if( p != 0 ){
				mFileTexture = str.substr( 0, p+1 );//ここまで文字列を抜いて
				string temp( &material[ i ].texture_file_name[ 0 ] );
				string tex;
				p = temp.find(".bmp");
				t = temp.find(".png");
				sph = temp.find(".sph");
				spa = temp.find(".spa");
				tga = temp.find(".tga");
				if( p > 0 ){
					tex = temp.substr( 0, p+1 );
					tex += "png";
				} else if( t > 0 ){
					tex = temp.substr( 0, t+1 );
					tex += "png";
				} else if( sph > 0 ){
					tex = temp.substr( 0, sph+1 );
					tex += "png";
				} else if( spa > 0 ){		
					tex = temp.substr( 0, spa+1 );
					tex += "png";	
				} else {
					tex = temp;
				}
				if( tga > 0 ){
					tex = temp.substr( 0, tga+1 );
					tex += "png";
				}
				if( temp.size() > 0 ){
					string str;
					str = mFileTexture;
					str += tex;
					mMaterial[ i ].mTexture = Graphics::Texture::create( str.c_str() );
				}
			}
		}
	}
};

}
}

#endif