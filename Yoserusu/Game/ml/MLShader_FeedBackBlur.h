


#ifndef MLSHADER_FEEDBACKBLUR_H_
#define MLSHADER_FEEDBACKBLUR_H_

#include "Mashiro\Graphics\RenderTarget.h"

using namespace Mashiro::Graphics;

namespace ML
{
class MLShader_FeedBack
{
public:
	MLShader_FeedBack();
	~MLShader_FeedBack();
	
	int Create();
	int Update();

private:
	int cRT_A();	//	RT_Aの作成
	int cRT_B();	//	RT_Bの作成
};
}//ml
#endif