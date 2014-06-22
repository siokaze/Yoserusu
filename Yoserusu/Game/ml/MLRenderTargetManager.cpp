
/*
	RTをそこかしこで作るとめんどくさそうなので全てのスロットのRTを管理するクラス
	に出来たら最高

*/


#include "MLRenderTargetManager.h"
#include <Mashiro\WorkSpace\WorkSpace.h>
#include <Mashiro\Graphics\GraphicsManager.h>

using namespace ML;

//	スロットに限界があったはずだけど分かんないから適当に定義
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
	//	既に中身が有った時はどうしよう？
	if(m_RT[slot] || slot > RT_SLOT_LIMIT)
		return false;

	//	RT作成
	m_RT[slot] = RenderTarget::create(
			Mashiro::WorkSpace::instance().width(), 
			Mashiro::WorkSpace::instance().height(), 
			Mashiro::Graphics::FORMAT_B8G8R8A8_UNORM);

	return true;
}

bool MLRenderTargetManager::setRenderTarget(int slot = 0)
{
	//	RTのセット
	//	中身のチェックは行わない
	//	実行頻度多分高いから
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