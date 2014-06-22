


#ifndef MLTESTAREA_H_
#define MLTESTAREA_H_

#include "Game\ml\MLCameraHandler.h"
#include "Game\ml\MLShader_FeedBackBlur.h"

namespace ML
{
class MLTestArea
{
public:
	MLTestArea();
	~MLTestArea();

	void Initialize();
	void Update();

private:
	MLCameraHandler m_camHandle;
	MLShader_FeedBack m_feedBack;
};
}
#endif