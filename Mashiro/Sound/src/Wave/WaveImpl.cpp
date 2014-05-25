#ifndef INCLUDE_SOUND_IMPL_H_
#define INCLUDE_SOUND_IMPL_H_

#include "GameLib/GameLib.h"
#include "GameLib/FileIO/Manager.h"
#include "GameLib/FileIO/InFile.h"
#include "GameLib/Base/Impl/ReferenceType.h"
#include "GameLib/Base/RefString.h"
#include <XAudio2.h>

#include "Sound/ヘッダー ファイル/Wave.h"
#include "Sound/ヘッダー ファイル/Mixer.h"
#include "Sound/ヘッダー ファイル/Player.h"
#include "DecoderBase.h"
#include "SoundDataCreate.h"
#include "SoundBase.h"

namespace Marisa {
namespace Sound{

//ソースボイスを作っちゃってプレイヤーに渡してあちら側で再生する
class Wave::Impl : public GameLib::ReferenceType {
public:
	Impl(){
	}
	~Impl(){
	}

};

};
};

#endif
