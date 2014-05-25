#ifndef INCLUDE_SOUND_MANAGER_H_
#define INCLUDE_SOUND_MANAGER_H_

#include "Mashiro/Sound/MusicSource.h"

namespace Mashiro {
namespace Sound{

class Wave;
class Mixer;

class Player {
public:
	//作成
	static Player create( MusicSource );
	//再生
	void start();
	//再生ループ
	void start( unsigned int roop, unsigned int lenght );
	//ストップ
	void stop();
	//ポーズ
	void pause();
	//音量
	void setVolum( float v );
	//ピッチ変更
	void setPitch( float s );

	//以下ユーザは意識しない関数群
	Player();
	Player( const Player& );
	~Player();
	operator void*() const;
	Player& operator=( const Player& );
	void release();
	bool operator==( const Player& ) const;
	bool operator!=( const Player& ) const;
private:
	//中身はこのなか
	class Impl;
	Impl* mImpl;
};

};
};

#endif