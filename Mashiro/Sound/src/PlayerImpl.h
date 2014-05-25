#ifndef INCLUDE_SOUND_IMPL_H_
#define INCLUDE_SOUND_IMPL_H_

#include "Mashiro/Mashiro.h"
#include "Mashiro/Base/Impl/ReferenceType.h"
#include <XAudio2.h>

#include "Mashiro/Sound/Player.h"
#include "Mashiro/Sound/MusicSource.h"
#include "Mashiro/Sound/src/MusicSourceImpl.h"

namespace Mashiro {
namespace Sound{

class Player::Impl : public Mashiro::ReferenceType {
public:
	Impl( MusicSource::Impl* s ) : mMusicSource( 0 ){
		setMusicSorce( s );
	}
	~Impl(){
		setMusicSorce( 0 );
	}

	void start(){
		HRESULT hr;
		XAUDIO2_BUFFER bufferInfo = { 0 };
		XAUDIO2_VOICE_STATE voiceState ;
		IXAudio2SourceVoice* voice = mMusicSource->getSource();


		voice->GetState( &voiceState );

		if( voiceState.BuffersQueued ){
			hr = SUCCEEDED( voice->Start() );
			return;
		}

		bufferInfo.Flags = XAUDIO2_END_OF_STREAM;
		bufferInfo.AudioBytes = mMusicSource->getBufferSize();
		bufferInfo.pAudioData = mMusicSource->getBuffer();

		voice->SubmitSourceBuffer( &bufferInfo );
		
		hr = SUCCEEDED( voice->Start() );
		return;
	}

	void start( unsigned int loopPos, unsigned int loopLength ){
		XAUDIO2_BUFFER bufferInfo = { 0 };
		XAUDIO2_VOICE_STATE voiceState ;
		IXAudio2SourceVoice* voice = mMusicSource->getSource();


		voice->GetState( &voiceState );

		if( voiceState.BuffersQueued ){
			SUCCEEDED( voice->Start() );
			return;
		}

		bufferInfo.Flags = XAUDIO2_END_OF_STREAM;
		bufferInfo.AudioBytes = mMusicSource->getBufferSize();
		bufferInfo.pAudioData = mMusicSource->getBuffer();
		bufferInfo.LoopBegin = loopPos;
		bufferInfo.LoopLength = loopLength;
		bufferInfo.LoopCount = XAUDIO2_LOOP_INFINITE;

		voice->SubmitSourceBuffer( &bufferInfo );

		voice->Start();
		return;
	}

	void setMusicSorce( MusicSource::Impl* o ){
		if( mMusicSource == o ){
			return;
		}
		if( mMusicSource ){
			mMusicSource->release();
			if( mMusicSource->referenceCount() == 0 ){
				SAFE_DELETE( mMusicSource );
			}
		}
		mMusicSource = o;
		if( o ){
			o->refer();
		}
	}

	void pause(){
		mMusicSource->getSource()->Stop();
	}

	void stop(){
		mMusicSource->getSource()->Stop();
		mMusicSource->getSource()->FlushSourceBuffers();
	}

	void setVolum( float volume ){
		if( volume >= 1.f ){
			volume = 1.f;
		} else if( volume <= 0.f ){
			volume = 0.f;
		}
		mMusicSource->getSource()->SetVolume( volume );
	}

	void setPitch( float s ){
		mMusicSource->getSource()->SetFrequencyRatio( s );
	}
private:
	MusicSource::Impl* mMusicSource;
};


};
};

#endif
