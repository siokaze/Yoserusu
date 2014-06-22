


#include <math.h>
#include "Game\ml\MLCameraAction_HeadBanging.h"

using namespace ML;

MLCameraAction_HeadBanging::MLCameraAction_HeadBanging()
	: MLCameraAction_Base(), m_count(0)
{
	
}

MLCameraAction_HeadBanging::~MLCameraAction_HeadBanging()
{
	
}

void MLCameraAction_HeadBanging::Initialize()
{
	m_count = 0;
}

Matrix* MLCameraAction_HeadBanging::Update()
{
	m_count += 3;

	float temp_rotX;
	
	temp_rotX = sin(3.14 / 60 * m_count) * 0.1f;
	m_mat.setRotationX(temp_rotX);

	return &m_mat;
}