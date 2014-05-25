#include "Mashiro/Mashiro.h"

#include "Mashiro/Sound/AudioEffect.h"
#include "Mashiro/Sound/src/AudioEffectImpl.h"
#include "Mashiro/Sound/MusicSource.h"

namespace Mashiro {
namespace Sound {

AudioEffect AudioEffect::create(){
	AudioEffect r;
	r.mImpl = NEW Impl();
	return r;
}

void AudioEffect::createEcho( float wet, float feed, float delay ){
	ASSERT( mImpl && "Sound::AudioEffect : This is empty Object" );
	mImpl->createEcho( wet, feed, delay );
}

void AudioEffect::createReverb( float diff, float room ){
	ASSERT( mImpl && "Sound::AudioEffect : This is empty Object" );
	mImpl->createReverb( diff, room );
}

void AudioEffect::createMasteringLimiter( unsigned int release, unsigned int lou ){
	ASSERT( mImpl && "Sound::AudioEffect : This is empty Object" );
	mImpl->createMasteringLimiter( release, lou );
}

void AudioEffect::createEq(
float f0, float g0, float b0,
float f1, float g1, float b1,
float f2, float g2, float b2,
float f3, float g3, float b3 ){
	ASSERT( mImpl && "Sound::AudioEffect : This is empty Object" );
	mImpl->createEq( f0, g0, b0, f1, g1, b1, f2, g2, b2, f3, g3, b3 );
}

void AudioEffect::setEffect( MusicSource source ){
	ASSERT( mImpl && "Sound::AudioEffect : This is empty Object" );
	mImpl->setEffect( source.mImpl );
}

#define TYPE AudioEffect
#include "Mashiro/Base/Impl/ReferenceTypeTemplate.h"

}
}
