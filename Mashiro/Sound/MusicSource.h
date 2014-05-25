#ifndef INCLUDE_MUSIC_SOURCE_H_
#define INCLUDE_MUSIC_SOURCE_H_

namespace Mashiro {
namespace Sound {

class MusicSource {
public:
	//音声ファイル生成
	static MusicSource create( const char* fileName );
	//バッファー
	unsigned char* getBuffer();
	//バッファーサイズ
	unsigned int getBufferSize();
	//エフェクトOn
	void onEffect();
	//エフェクトoff
	void offEffect();

	//以下ユーザは意識しない関数群
	MusicSource();
	MusicSource( const MusicSource& );
	~MusicSource();
	operator void*() const;
	MusicSource& operator=( const MusicSource& );
	void release();
	bool operator==( const MusicSource& ) const;
	bool operator!=( const MusicSource& ) const;
	class Impl;
private:
	friend class Player;
	friend class AudioEffect;
	Impl* mImpl;
};

}
}

#endif
