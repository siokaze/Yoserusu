#ifndef INCLUDE_MASHIRO_SOUND_AUDIO_EFFECT_H_
#define INCLUDE_MASHIRO_SOUND_AUDIO_EFFECT_H_

#include "Mashiro/Sound/EffectParameter.h"
#include "Mashiro/Sound/MusicSource.h"

namespace Mashiro {
namespace Sound {

class AudioEffect {
public:
	enum Effect{
		EFFECT_REVERB,
		EFFECT_ECHO,
		EFFECT_EQ,
		EFFECT_LIMITER,

		EFFECT_VOLUME,

		EFFECT_NONE,
	};

	//生成
	static AudioEffect create();
	//エフェクト生成
	void createEcho( float wetDryMix = FXECHO_DEFAULT_WETDRYMIX, float feedback = FXECHO_DEFAULT_FEEDBACK, float delay = FXECHO_DEFAULT_DELAY  );
	void createReverb( float diffuse = FXREVERB_DEFAULT_DIFFUSION, float roomSize = FXREVERB_DEFAULT_ROOMSIZE );
	void createMasteringLimiter( unsigned int release = FXMASTERINGLIMITER_DEFAULT_RELEASE, unsigned int loudness = FXMASTERINGLIMITER_DEFAULT_LOUDNESS );
	void createEq( 
		float frequencyCenter0 = FXEQ_DEFAULT_FREQUENCY_CENTER_0, float gain0 = FXEQ_DEFAULT_GAIN, float bandwidth0 = FXEQ_DEFAULT_BANDWIDTH,
		float frequencyCenter1 = FXEQ_DEFAULT_FREQUENCY_CENTER_1, float gain1 = FXEQ_DEFAULT_GAIN, float bandwidth1 = FXEQ_DEFAULT_BANDWIDTH,
		float frequencyCenter2 = FXEQ_DEFAULT_FREQUENCY_CENTER_2, float gain2 = FXEQ_DEFAULT_GAIN, float bandwidth2 = FXEQ_DEFAULT_BANDWIDTH,
		float frequencyCenter3 = FXEQ_DEFAULT_FREQUENCY_CENTER_3, float gain3 = FXEQ_DEFAULT_GAIN, float bandwidth3 = FXEQ_DEFAULT_BANDWIDTH );
	//エフェクトセット
	void setEffect( MusicSource );

	//以下ユーザは意識しない関数群
	AudioEffect();
	AudioEffect( const AudioEffect& );
	~AudioEffect();
	operator void*() const;
	AudioEffect& operator=( const AudioEffect& );
	void release();
	bool operator==( const AudioEffect& ) const;
	bool operator!=( const AudioEffect& ) const;
private:
	class Impl;
	Impl* mImpl;
};

}
}

#endif