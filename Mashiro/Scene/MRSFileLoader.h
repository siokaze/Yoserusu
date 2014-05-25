#ifndef MASHIRO_GRAPHICS_MRS_FILE_LOADER_H_
#define MASHIRO_GRAPHICS_MRS_FILE_LOADER_H_

namespace Mashiro{

namespace Scene {

class Model;
class AnimationModel;

class MRSFileLoader{
public:
	//生成
	static MRSFileLoader create();
	//Xファイルからモデルを作ります
	AnimationModel createModel( const char* fileName );
	//モデル
	Model createStaticModel( const char* filename );

	//ロード終了チェック
	bool isReady() const;

	//以下ユーザは気にするな
	MRSFileLoader();
	~MRSFileLoader();
	MRSFileLoader( const MRSFileLoader& );
	operator void*() const;
	MRSFileLoader& operator=( const MRSFileLoader& );
	void release();
	bool operator==( const MRSFileLoader& ) const;
	bool operator!=( const MRSFileLoader& ) const;
	class Impl;
	explicit MRSFileLoader( const Impl* );
private:
	friend class AnimationModel;
	Impl* mImpl;
};

}
}

#endif
