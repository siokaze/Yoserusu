#ifndef INCLUDE_MUSIC_SOURCE_IMPL_H_
#define INCLUDE_MUSIC_SOURCE_IMPL_H_

#include "Mashiro/Mashiro.h"
#include "Mashiro/Base/Impl/ReferenceType.h"
#include <XAudio2.h>
#include <XAPOFX.h>

#include "Mashiro/Sound/MusicSource.h"
#include "Mashiro/Sound/src/DecoderBase.h"
#include "Mashiro/Sound/src/SoundDataCreate.h"
#include "Mashiro/Sound/src/SoundBase/SoundBase.h"

namespace Mashiro {
namespace Sound{

class MusicSource::Impl : public Mashiro::ReferenceType {
public:
	Impl( const char* fileName ) : 
	  mFileName( fileName ),
	  mSourceVoice( NULL ),
	  mBuffer( 0 ),
	  mBufferSize( 0 ),
	  mDecoder( 0 ){
		  create();
	}
	~Impl(){
		if( mSourceVoice == 0 ){
			return;
		}

		mSourceVoice->Stop();

		mSourceVoice->DestroyVoice();
		mSourceVoice = 0;

		SAFE_DELETE_ARRAY( mBuffer );
	}
	bool create(){
		IXAudio2* device = SoundBase::instance().getDevice();
		mDecoder = createDecoder( mFileName );
		XAUDIO2_VOICE_SENDS* sends = 0;
		WAVEFORMATEX format;

		if( mDecoder == 0 ){
			return false;
		}

		if( !mDecoder->open( mFileName, format ) ){
			return false;
		}

		if( FAILED( device->CreateSourceVoice(
			&mSourceVoice,
			&format,
			0,
			4.0f,
			0,
			sends,
			0 ) ) ){
				return false;
		}

		mBufferSize = format.nBlockAlign * mDecoder->getLength();
		mBuffer = NEW unsigned char[ mBufferSize ];

		unsigned char* tempBuf = mBuffer;
		unsigned int tempSize = mBufferSize;
		unsigned int tempResult = 0;

		do{
			tempResult = mDecoder->decode( tempBuf, tempSize );

			tempSize -= tempResult;
			tempBuf += tempResult;
		}
		while( tempResult );

		SAFE_DELETE( mDecoder );

		return true;
	}
	IXAudio2SourceVoice*& getSource(){
		return mSourceVoice;
	}

	unsigned char* getBuffer(){
		return mBuffer;
	}

	unsigned int getBufferSize(){
		return mBufferSize;
	}

	void onEffect(){
		mSourceVoice->EnableEffect( 0 );
	}

	void offEffect(){
		mSourceVoice->DisableEffect( 0 );
	}

private:
	const char* mFileName;

	IXAudio2SourceVoice* mSourceVoice;

	unsigned char* mBuffer;
	unsigned int mBufferSize;

	DecoderBase* mDecoder;
};

}
}

#endif
