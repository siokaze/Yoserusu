#include "Mashiro/Mashiro.h"

#include "Mashiro/Sound/MusicSource.h"
#include "Mashiro/Sound/src/MusicSourceImpl.h"

namespace Mashiro {
namespace Sound {

MusicSource MusicSource::create( const char* fileName ){
	MusicSource r;
	r.mImpl = NEW Impl( fileName );
	return r;
}

unsigned int MusicSource::getBufferSize(){
	ASSERT( mImpl && "Sound::MusicSource : This is empty Object" );
	return mImpl->getBufferSize();
}

unsigned char* MusicSource::getBuffer(){
	ASSERT( mImpl && "Sound::MusicSource : This is empty Object" );
	return mImpl->getBuffer();
}

void MusicSource::onEffect(){
	ASSERT( mImpl && "Sound::MusicSource : This is empty Object" );
	return mImpl->onEffect();
}

void MusicSource::offEffect(){
	ASSERT( mImpl && "Sound::MusicSource : This is empty Object" );
	return mImpl->offEffect();
}

#define TYPE MusicSource
#include "Mashiro/Base/Impl/ReferenceTypeTemplate.h"

}
}