#ifndef INCLUDE_UTILITY_SOUNDMANAGER_H_
#define INCLUDE_UTILITY_SOUNDMANAGER_H_

#include "Mashiro/Sound/MusicSource.h"
#include "Mashiro/Sound/Player.h"

//サウンドクラス(シングルトン)
class SoundManager {
public:
	enum Bgm{
		BGM_TITLE,
		BGM_GAMEPLAY,
		BGM_ENDING,
		BGM_AUTHE,

		BGM_MAX,
	};
	enum Se{
		SE_OK,
		SE_OPEN,
		SE_FALSE,
		SE_COUNTDOWN,
		SE_START,
		SE_WRNNING,
		SE_END,

		SE_MAX,
	};
public:
	static SoundManager* instance();
	static void create();
	static void destory();

	bool hasLoaded();

	void playBgm( Bgm bgm );
	void stopBgm();

	void playSe( Se se );
	void setPitch( float p );
private:
	SoundManager();
	~SoundManager();
	static SoundManager* mInstance;

	Mashiro::Sound::MusicSource* mBgmWaves;
	Mashiro::Sound::MusicSource* mSeWaves;
	//Player
	Mashiro::Sound::Player mBgmPlayer;
	static const int SE_PLAYER_MAX = 8;
	Mashiro::Sound::Player mSePlayers[ SE_PLAYER_MAX ];
	int mSePlayerPos;
};

#endif