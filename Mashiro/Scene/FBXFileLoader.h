#ifndef MASHIRO_GRAPHICS_FBX_FILE_LOADER_H_
#define MASHIRO_GRAPHICS_FBX_FILE_LOADER_H_

namespace Mashiro{

namespace Graphics{
	class Texture;
}

namespace Scene {

class Model;

class FBXFileLoader{
public:
	//生成
	static FBXFileLoader create();
	//FBXファイルからモデルを作ります
	Model createModel( const char* fileName );

	//ロード終了チェック
	bool isReady() const;

	//以下ユーザは気にするな
	FBXFileLoader();
	~FBXFileLoader();
	FBXFileLoader( const FBXFileLoader& );
	operator void*() const;
	FBXFileLoader& operator=( const FBXFileLoader& );
	void release();
	bool operator==( const FBXFileLoader& ) const;
	bool operator!=( const FBXFileLoader& ) const;
	class Impl;
	explicit FBXFileLoader( const Impl* );
private:
	Impl* mImpl;
};

}
}

#endif
