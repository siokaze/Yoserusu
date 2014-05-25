#ifndef INCLUDE_SOUND_BASE_IMPL_H_
#define INCLUDE_SOUND_BASE_IMPL_H_

#include "Mashiro/Mashiro.h"
#include <XAudio2.h>

#include "Mashiro/Sound/src/SoundBase/SoundBase.h"

namespace Mashiro {
namespace Sound {

class SoundBaseImpl{
public:
	SoundBaseImpl() :
	mXAudio2( NULL ),
	mMasteringVoice( NULL ){
		HRESULT hr = S_OK;
		//複数生成してはならない
		if( mXAudio2 ){
			ASSERT( "XAudio2を複数生成してます" );
		}
		if( mMasteringVoice ){
			ASSERT( "MasteringVoiceを複数生成してます" );
		}
		//	XAudio2を初期化
	UINT32 flags = 0;
#ifdef _DEBUG
	flags |= XAUDIO2_DEBUG_ENGINE;
#endif
		//XAudio2作成
		hr = XAudio2Create( &mXAudio2, flags );
		if( FAILED( hr ) ) {
			hr = E_FAIL;
			ASSERT( hr != E_FAIL && "XAudio2の生成に失敗しました" );
		}

		//Mastering Voice作成
		hr = mXAudio2->CreateMasteringVoice( &mMasteringVoice );
		if( FAILED( hr ) ){
			if( mXAudio2 ) mXAudio2->Release();
			hr = E_FAIL;
			ASSERT( hr != E_FAIL && "MasteringVoiceの生成に失敗しました" );
		}
	}
	~SoundBaseImpl(){
		mMasteringVoice->DestroyVoice();
		mMasteringVoice = 0;
		mXAudio2->Release();
		mXAudio2 = 0;
	}

	IXAudio2*& getDevice(){
		return mXAudio2;
	}

	IXAudio2MasteringVoice*& getVoice(){
		return mMasteringVoice;
	}
private:
	//XAudio2 再生やらなにやら
	IXAudio2* mXAudio2;
	//マスターVoice
	IXAudio2MasteringVoice* mMasteringVoice;
};

};
};


#endif
