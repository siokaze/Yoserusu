
/*

	カメラにヘドバンさせる謎のクラス
*/


#ifndef MLCAMERAACTION_HEADBANGING_H_
#define MLCAMERAACTION_HEADBANGING_H_

#include "Game\ml\MLCameraAction_Base.h"

using namespace Mashiro::Math;

namespace ML
{
class MLCameraAction_HeadBanging : public MLCameraAction_Base
{
public:
	MLCameraAction_HeadBanging();
	~MLCameraAction_HeadBanging();

	void Initialize() override;
	Matrix* Update() override;

private:
	int m_count;
};
}
#endif