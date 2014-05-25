#ifndef INCLUDE_AUDIOEFFECT_IMPL_H_
#define INCLUDE_AUDIOEFFECT_IMPL_H_

#include <XAPOFX.h>
#pragma comment( lib, "XAPOFX.lib" )

#include "Mashiro/Mashiro.h"
#include "Mashiro/Base/Impl/ReferenceType.h"
#include <XAudio2.h>

#include "Mashiro/Sound/AudioEffect.h"
#include "Mashiro/Sound/src/MusicSourceImpl.h"
#include "Mashiro/Sound/EffectParameter.h"

namespace Mashiro {
namespace Sound{

class AudioEffect::Impl : public Mashiro::ReferenceType {
public:
	Impl(){
	}
	~Impl(){
		mEffect->Release();
		mEffect = 0;
	}
	bool createEcho(
		float wetDryMix,
		float feedBack,
		float delay ){
			mMode = AudioEffect::EFFECT_ECHO;
			if( FAILED( ::CreateFX( __uuidof( ::FXEcho ), &mEffect ) ) ){
				return false;
			}
			//パラメータの設定
			mEcho.WetDryMix = wetDryMix;
			mEcho.Feedback = feedBack;
			mEcho.Delay = delay;
			return true;
	}
	bool createReverb( 
		float diffuse,
		float roomSize ){
			mMode = AudioEffect::EFFECT_REVERB;
			if( FAILED(::CreateFX( __uuidof( ::FXReverb ), &mEffect) ) ){
				return false;
			}
			//パラメータの設定もします
			mReverb.Diffusion = diffuse;
			mReverb.RoomSize = roomSize;
			return true;
	}
	bool createMasteringLimiter(
		unsigned int release,
		unsigned int loudness ){
			mMode = AudioEffect::EFFECT_LIMITER;
			if( FAILED( ::CreateFX( __uuidof( ::FXMasteringLimiter ), &mEffect ) ) ){
				return false;
			}
			//パラメータの設定もします
			mMaster.Release = release;
			mMaster.Loudness = loudness;
			return true;
	}
	bool createEq(	
		float frequencyCenter0, float gain0, float bandwidth0,
		float frequencyCenter1, float gain1, float bandwidth1,
		float frequencyCenter2, float gain2, float bandwidth2,
		float frequencyCenter3, float gain3, float bandwidth3 ){
			mMode = AudioEffect::EFFECT_EQ;			
			if( FAILED( ::CreateFX( __uuidof( ::FXEQ ), &mEffect ) ) ){
				return false;
			}
			//パラメータの設定もします
			mEq.FrequencyCenter0 = frequencyCenter0;
			mEq.FrequencyCenter1 = frequencyCenter1;
			mEq.FrequencyCenter2 = frequencyCenter2;
			mEq.FrequencyCenter3 = frequencyCenter3;
			mEq.Bandwidth0 = bandwidth0;
			mEq.Bandwidth1 = bandwidth1;
			mEq.Bandwidth2 = bandwidth2;
			mEq.Bandwidth3 = bandwidth3;
			mEq.Gain0 = gain0;
			mEq.Gain1 = gain1;
			mEq.Gain2 = gain2;
			mEq.Gain3 = gain3;
			return true;	
	}
	void setEffect( MusicSource::Impl* source ){
		//エフェクトと接続します
		XAUDIO2_EFFECT_DESCRIPTOR des;
		des.InitialState = true;
		des.OutputChannels = 1;
		des.pEffect = mEffect;
		IXAudio2SourceVoice* voice = source->getSource();

		XAUDIO2_EFFECT_CHAIN chain;
		chain.EffectCount = 1;
		chain.pEffectDescriptors = &des;

		voice->SetEffectChain( &chain );
		//パラ写しifだらけでみにくいわ
		if( mMode == AudioEffect::EFFECT_ECHO ){
			if( HRESULT( source->getSource()->SetEffectParameters( 0, &mEcho, sizeof( mEcho ) ) ) ){
				return;
			}
		}
		if( mMode == AudioEffect::EFFECT_LIMITER ) {
			if( HRESULT( source->getSource()->SetEffectParameters( 0, &mMaster, sizeof( mMaster ) ) ) ){
				return;
			}
		}
		if( mMode == AudioEffect::EFFECT_EQ ){
			if( HRESULT( source->getSource()->SetEffectParameters( 0, &mEq, sizeof( mEq ) ) ) ){
				return;
			}
		}
		if( mMode == AudioEffect::EFFECT_REVERB ){
			if( HRESULT( source->getSource()->SetEffectParameters( 0, &mReverb, sizeof( mReverb ) ) ) ){
				return;
			}	
		}
		//解放
		mEffect->Release();
	}
private:
	//Unkonwn型にエフェクトしまいます
	::IUnknown* mEffect;
	FXREVERB_PARAMETERS mReverb;
	FXECHO_PARAMETERS mEcho;
	FXEQ_PARAMETERS mEq;
	FXMASTERINGLIMITER_PARAMETERS mMaster;
	//どのエフェクト？
	AudioEffect::Effect mMode;
};

}
}

#endif