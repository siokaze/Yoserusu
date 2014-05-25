#ifndef MASHIRO_GRAPHICS_FBX_SKIN_MESH_LOADER_H_
#define MASHIRO_GRAPHICS_FBX_SKIN_MESH_LOADER_H_

namespace Mashiro{

namespace Scene {

class AnimationModel;

class FBXSkinMeshLoader{
public:
	//生成
	static FBXSkinMeshLoader create();
	//FBXファイルからモデルを作ります
	AnimationModel createModel( const char* fileName );

	void createMRSModelFile( const char* fileName );
	void createSINAnimationFile( const char* fileName, int frame1, int frame2 );

	//ロード終了チェック
	bool isReady() const;

	//以下ユーザは気にするな
	FBXSkinMeshLoader();
	~FBXSkinMeshLoader();
	FBXSkinMeshLoader( const FBXSkinMeshLoader& );
	operator void*() const;
	FBXSkinMeshLoader& operator=( const FBXSkinMeshLoader& );
	void release();
	bool operator==( const FBXSkinMeshLoader& ) const;
	bool operator!=( const FBXSkinMeshLoader& ) const;
	class Impl;
	explicit FBXSkinMeshLoader( const Impl* );
private:
	friend class AnimationModel;
	Impl* mImpl;
};

}
}

#endif
