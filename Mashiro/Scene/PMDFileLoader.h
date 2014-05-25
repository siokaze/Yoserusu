#ifndef MASHIRO_GRAPHICS_PMD_FILE_LOADER_H_
#define MASHIRO_GRAPHICS_PMD_FILE_LOADER_H_

namespace Mashiro{

namespace Scene {

class Model;

class PMDFileLoader{
public:
	//生成
	static PMDFileLoader create();
	//Xファイルからモデルを作ります
	Model createModel( const char* fileName );

	//ロード終了チェック
	bool isReady() const;

	//以下ユーザは気にするな
	PMDFileLoader();
	~PMDFileLoader();
	PMDFileLoader( const PMDFileLoader& );
	operator void*() const;
	PMDFileLoader& operator=( const PMDFileLoader& );
	void release();
	bool operator==( const PMDFileLoader& ) const;
	bool operator!=( const PMDFileLoader& ) const;
	class Impl;
	explicit PMDFileLoader( const Impl* );
private:
	friend class CollisionMesh;
	Impl* mImpl;
};

}
}

#endif
