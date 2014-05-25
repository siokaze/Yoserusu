#ifndef INCLUDE_SOUND_BASE_H_
#define INCLUDE_SOUND_BASE_H_

#include <XAudio2.h>

namespace Mashiro {
namespace Sound {

class SoundBase{
public:
	SoundBase();
	static SoundBase instance();

	//デバイスゲット
	IXAudio2*& getDevice();
	//マスターボイスゲット
	IXAudio2MasteringVoice*& getMasterVoice();

	//生成だけ
	static void create();
	static void destroy();
};

}
}

#endif
