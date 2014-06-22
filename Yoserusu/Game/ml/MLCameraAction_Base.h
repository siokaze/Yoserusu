
/*

	�J�����̋����ɂ��ẴN���X�x�[�X
	�p�����Ă܂ł�邱�Ƃ͓��ɂȂ����ꉞ�Ǘ��N���X��
	�S�Ă̍s�����Ǘ�����̂ł�����y�ɂ��邽�ߐ錾

*/


#ifndef MLCAMERAACTOIN_BASE_H_
#define MLCAMERAACTOIN_BASE_H_

#include"Mashiro\Math\Matrix.h"

namespace ML
{
class MLCameraAction_Base
{
public:
	MLCameraAction_Base();
	virtual ~MLCameraAction_Base();

	virtual void Initialize() = 0;
	virtual Mashiro::Math::Matrix* Update() = 0;

protected:
	Mashiro::Math::Matrix m_mat;
};
}
#endif