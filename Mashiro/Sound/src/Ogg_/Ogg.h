#ifndef INCLUDE_MASHIRO_OGG_H_
#define INCLUDE_MASHIRO_OGG_H_

#include <XAudio2.h>
#include <stdio.h>
#include "vorbis/vorbisfile.h"
#include "Mashiro/Sound/src/DecoderBase.h"

namespace Mashiro {
namespace Sound{

class Ogg : public DecoderBase{
public:
	Ogg();
	~Ogg();

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
	bool readFormat( FILE* fp, WAVEFORMATEX& format );
private:
	OggVorbis_File mFp;
	bool mOpen;
};

};
};

#endif