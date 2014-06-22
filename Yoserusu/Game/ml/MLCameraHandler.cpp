

#include "MLCameraHandler.h"
#include "Game\ml\MLCameraAction_HeadBanging.h"
#include <algorithm>

using namespace ML;
using namespace Mashiro;
using namespace Mashiro::Math;

namespace 
{
	static Math::Matrix camMat;
}

MLCameraHandler::MLCameraHandler()
	: m_updateTarget(nullptr)
{

	m_actArray = new MLCameraAction_Base*[2];
	m_actArray[ACTION_TYPE::HEAD_BANGING] = new MLCameraAction_HeadBanging();
	m_actArray[1] = new MLCameraAction_HeadBanging();

	//m_updateTarget = m_actArray[1];
}

MLCameraHandler::~MLCameraHandler()
{
	if(m_actArray[ACTION_TYPE::HEAD_BANGING]) 
		delete m_actArray[ACTION_TYPE::HEAD_BANGING];
	if(m_actArray[1]) 
		delete m_actArray[1];

	delete[] m_actArray;
}

void MLCameraHandler::Update()
{	
	if(m_updateTarget)
	{
		//	計算クラスからの数値と合成
		camMat *= (*m_updateTarget->Update());
		//	行列のセット
		Graphics::Manager::instance().setViewMatrix(camMat);
	}
}

void MLCameraHandler::actSet(ACTION_TYPE act)
{
	//	中身が無いときにのみターゲットをセットするか
	//	どうかは検討中
	if(!m_updateTarget)
	{
		m_updateTarget = m_actArray[act];
		m_updateTarget->Initialize();
	}
}

void MLCameraHandler::actRemove()
{
	if(m_updateTarget)
	{
		m_updateTarget->Initialize();
		m_updateTarget = nullptr;
	}
}

void MLCameraHandler::setCameraMatrix(Math::Matrix* pCamMat)
{
	camMat = (*pCamMat);
}

Math::Matrix& MLCameraHandler::getCameraMatrix()
{
	return camMat;
}

