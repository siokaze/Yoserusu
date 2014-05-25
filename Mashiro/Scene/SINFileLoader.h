#ifndef MASHIRO_GRAPHICS_SIN_FILE_LOADER_H_
#define MASHIRO_GRAPHICS_SIN_FILE_LOADER_H_

namespace Mashiro{

namespace Scene {

class AnimationModel;

class SINFileLoader{
public:
	//生成
	static SINFileLoader create( const char* fileName );

	//ロード終了チェック
	bool isReady() const;

	//以下ユーザは気にするな
	SINFileLoader();
	~SINFileLoader();
	SINFileLoader( const SINFileLoader& );
	operator void*() const;
	SINFileLoader& operator=( const SINFileLoader& );
	void release();
	bool operator==( const SINFileLoader& ) const;
	bool operator!=( const SINFileLoader& ) const;
	class Impl;
	explicit SINFileLoader( const Impl* );
private:
	friend class AnimationModel;
	Impl* mImpl;
};

}
}

#endif
