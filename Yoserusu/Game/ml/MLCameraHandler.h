


/*

	setCamMatでカメラの行列を保持しておくことでこのクラス内での
	カメラの制御を直感的にする

*/

#ifndef MLCAMERAHANDLER_H_
#define MLCAMERAHANDLER_H_

#include "Mashiro/Math/Matrix.h"
#include "Mashiro/Graphics/GraphicsManager.h"

#include "Game\ml\MLCameraAction_Base.h"

using namespace Mashiro;

namespace ML
{
enum ACTION_TYPE{
	HEAD_BANGING,	//	カメラを縦に振るだけ		
};

class MLCameraHandler
{
public:
	MLCameraHandler();
	~MLCameraHandler();

	void Update();

	//	実行するアクションの指定
	void actSet(ACTION_TYPE act);
	//	実行中のアクションの停止
	void actRemove();

	//	どうしようもなくfuckin静的関数
	static void setCameraMatrix(Math::Matrix* pCamMat);
	
	Math::Matrix& getCameraMatrix();

private:
	MLCameraAction_Base* m_updateTarget;	//	現在更新すべきアクション
	MLCameraAction_Base** m_actArray;		//	更新候補のアクション
};
}
#endif