#include "Mashiro/Sound/src/SoundBase/SoundBase.h"
#include "Mashiro/Sound/src/SoundBase/SoundBaseImpl.h"
#include "Mashiro/Mashiro.h"

#include "Mashiro/Window/Window.h"

namespace Mashiro {
namespace Sound {

SoundBaseImpl* gSoundBaseImpl = 0;

SoundBase::SoundBase(){
	//•ÊƒXƒŒƒbƒh‚©‚çNG
	ASSERT( Window::isMainThread() && "you must call from MAIN thread" );
}

void SoundBase::create(){
	STRONG_ASSERT( !gSoundBaseImpl );
	gSoundBaseImpl = NEW SoundBaseImpl();
}

IXAudio2*& SoundBase::getDevice(){
	return gSoundBaseImpl->getDevice();
}

IXAudio2MasteringVoice*& SoundBase::getMasterVoice(){
	return gSoundBaseImpl->getVoice();
}

void SoundBase::destroy() {
	SAFE_DELETE( gSoundBaseImpl );
}

SoundBase SoundBase::instance(){
	return SoundBase();
}

}
}