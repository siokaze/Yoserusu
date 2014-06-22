
/*
	RT�������������ō��Ƃ߂�ǂ��������Ȃ̂őS�ẴX���b�g��RT���Ǘ�����N���X
	�ɏo������ō�

*/


#include "MLRenderTargetManager.h"
#include <Mashiro\WorkSpace\WorkSpace.h>
#include <Mashiro\Graphics\GraphicsManager.h>

using namespace ML;

//	�X���b�g�Ɍ��E���������͂������Ǖ�����Ȃ�����K���ɒ�`
#define RT_SLOT_LIMIT (7)

MLRenderTargetManager::MLRenderTargetManager()
{
	
}

MLRenderTargetManager::~MLRenderTargetManager()
{
	for(std::vector<RenderTarget>::iterator t_itRT = m_RT.begin(); t_itRT != m_RT.end(); t_itRT++)
	{
		t_itRT->release();
	}
	m_RT.clear();
}

bool MLRenderTargetManager::CreateRenderTarget(int slot = 0)
{
	//	���ɒ��g���L�������͂ǂ����悤�H
	if(m_RT[slot] || slot > RT_SLOT_LIMIT)
		return false;

	//	RT�쐬
	m_RT[slot] = RenderTarget::create(
			Mashiro::WorkSpace::instance().width(), 
			Mashiro::WorkSpace::instance().height(), 
			Mashiro::Graphics::FORMAT_B8G8R8A8_UNORM);

	return true;
}

bool MLRenderTargetManager::setRenderTarget(int slot = 0)
{
	//	RT�̃Z�b�g
	//	���g�̃`�F�b�N�͍s��Ȃ�
	//	���s�p�x������������
	Manager::instance().setRenderTarget(slot, &m_RT[slot]);

	return true;
}

bool MLRenderTargetManager::ScreenShot(int slot = 0, const char* name = "default.png")
{
	if(m_RT[slot] || slot > RT_SLOT_LIMIT)
		return false;

	m_RT[slot].targetTexture(name);
	return true;
}

void MLRenderTargetManager::setBackBuffer()
{
	Manager::instance().setBackBufferTarget();
}