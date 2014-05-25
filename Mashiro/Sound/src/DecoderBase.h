#ifndef INCLUDE_SOUND_DECODER_BASE_H_
#define INCLUDE_SOUND_DECODER_BASE_H_

#include <XAudio2.h>

namespace Mashiro {
namespace Sound {

class DecoderBase {
public:
	virtual ~DecoderBase(){ }

	//オープン
	virtual bool open( const char* fileName, WAVEFORMATEX& format ) = 0;

	//クローズ
	virtual void rlease() = 0;

	//デコード
	virtual unsigned int decode( unsigned char* buffer, unsigned int size ) = 0;

	//再生位置を設定
	virtual void seek( int offset ) = 0;

	//終点ですか？
	virtual bool checkStreamEnd() = 0;

	//長さどんくらい？
	virtual unsigned int getLength() = 0;
};

}
}

#endif