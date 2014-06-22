


/*

	setCamMat�ŃJ�����̍s���ێ����Ă������Ƃł��̃N���X���ł�
	�J�����̐���𒼊��I�ɂ���

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
	HEAD_BANGING,	//	�J�������c�ɐU�邾��		
};

class MLCameraHandler
{
public:
	MLCameraHandler();
	~MLCameraHandler();

	void Update();

	//	���s����A�N�V�����̎w��
	void actSet(ACTION_TYPE act);
	//	���s���̃A�N�V�����̒�~
	void actRemove();

	//	�ǂ����悤���Ȃ�fuckin�ÓI�֐�
	static void setCameraMatrix(Math::Matrix* pCamMat);
	
	Math::Matrix& getCameraMatrix();

private:
	MLCameraAction_Base* m_updateTarget;	//	���ݍX�V���ׂ��A�N�V����
	MLCameraAction_Base** m_actArray;		//	�X�V���̃A�N�V����
};
}
#endif