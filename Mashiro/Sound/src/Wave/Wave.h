#ifndef INCLUDE_WAVE_H_
#define INCLUDE_WAVE_H_

#include <XAudio2.h>
#include <stdio.h>

#include "Mashiro/Sound/src/DecoderBase.h"

namespace Mashiro {
namespace Sound{

class Wave : public DecoderBase{
public:
	Wave();
	~Wave();

	//ファイルオープン
	bool open( const char* path, WAVEFORMATEX& format );
	//ファイルクローズ
	void close();
	//解放
	void rlease();
	//デコード
	unsigned int decode( unsigned char* buffer, unsigned int size );
	//再生位置を設定
	void seek( int offset );
	//サウンドファイルの終端チェック
	bool checkStreamEnd();
	//長さを取得
	unsigned int getLength();


	//フォーマットを読み取る
	bool readFormat( WAVEFORMATEX& format );
private:
	class Impl;
	Impl* mImpl;
	FILE* mFp;
	size_t mSoundSize;
	size_t mHeaderSize;
	size_t mBlockSize;
	size_t mLength;
};

};
};

#endif