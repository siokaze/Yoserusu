


#include "MLTestArea.h"
#include <Mashiro\Input\InputManager.h>
#include "Mashiro/Input/Keyboard.h"

using namespace Mashiro;
using namespace Mashiro::Input;

using namespace ML;

MLTestArea::MLTestArea()
{
	
}

MLTestArea::~MLTestArea()
{
	
}

void MLTestArea::Initialize()
{
	//m_feedBack.Create();
}

void MLTestArea::Update()
{
	//	camera--------------------------------------------------
	m_camHandle.Update();
	if( Mashiro::Input::Manager::instance().keyboard().isOn(Input::Keyboard::KEY_UP) ){
		m_camHandle.actSet(ML::ACTION_TYPE::HEAD_BANGING);
	}
	if( Mashiro::Input::Manager::instance().keyboard().isOn(Input::Keyboard::KEY_DOWN) ){
		m_camHandle.actRemove();
	}

	////	feedbackblur---------------------------------------------
	//if( Mashiro::Input::Manager::instance().keyboard().isOn(Input::Keyboard::KEY_RETURN) ){
	//	m_feedBack.Update();
	//}
}