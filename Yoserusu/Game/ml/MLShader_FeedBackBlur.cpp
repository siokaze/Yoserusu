


#include "MLShader_FeedBackBlur.h"
#include <Mashiro\WorkSpace\WorkSpace.h>
#include <Mashiro\Graphics\GraphicsManager.h>
#include "Game\ml\MLRenderTargetManager.h"

using namespace ML;
using namespace Mashiro;

MLShader_FeedBack::MLShader_FeedBack()
{
	
}

MLShader_FeedBack::~MLShader_FeedBack()
{

}

int MLShader_FeedBack::Create()
{
	RT_MANAGER->CreateRenderTarget(0);
	RT_MANAGER->CreateRenderTarget(1);
	return 1;
}

int MLShader_FeedBack::Update()
{
	this->cRT_A();
	this->cRT_B();
	return 1;
}

int MLShader_FeedBack::cRT_A()
{
	Manager::instance().setTexture(*RT_MANAGER->getRenderTarget(0), 1);



	return 1;
}

int MLShader_FeedBack::cRT_B()
{
	
	return 1;
}