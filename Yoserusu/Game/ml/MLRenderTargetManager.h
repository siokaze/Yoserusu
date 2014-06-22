


#ifndef MLRENDERTARGETMANAGER_H_
#define MLRENDERTARGETMANAGER_H_

#include <vector>
#include "Mashiro\Graphics\RenderTarget.h"

using namespace Mashiro;
using namespace Mashiro::Graphics;

namespace ML
{

//	‚ß‚ñ‚Ç‚¢‚Æ‚«—p
#define RT_MANAGER MLRenderTargetManager::getInstance()

class MLRenderTargetManager
{
public:
	static MLRenderTargetManager* getInstance()
	{
		static MLRenderTargetManager inst;
		return &inst;
	}

	bool CreateRenderTarget(int slot);
	bool setRenderTarget(int slot);
	RenderTarget* getRenderTarget(int slot){ return &m_RT[slot]; }
	bool ScreenShot(int slot, const char* name);
	void setBackBuffer();

private:
	MLRenderTargetManager();
	~MLRenderTargetManager();

private:
	std::vector<RenderTarget> m_RT;
};
}//ml
#endif