
/*

	カメラの挙動についてのクラスベース
	継承してまでやることは特にないが一応管理クラスで
	全ての行動を管理するのでそれを楽にするため宣言

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