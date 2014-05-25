#ifndef INCLUDE_MASHIRO_GRAPHICS_RENDERTARGET_H_
#define INCLUDE_MASHIRO_GRAPHICS_RENDERTARGET_H_

#include "Mashiro/Graphics/Enum.h"

namespace Mashiro{
namespace Graphics {

class RenderTarget {
public:
	static RenderTarget create( int w, int h, RenderFormat textureFormat );

	//レンダリングターゲットのテクスチャーをpngで保存します
	void targetTexture( const char* name );

	//以下ユーザは気にするな
	RenderTarget();
	~RenderTarget();
	RenderTarget( const RenderTarget& );
	operator void*() const;
	RenderTarget& operator=( const RenderTarget& );
	void release();
	bool operator==( const RenderTarget& ) const;
	bool operator!=( const RenderTarget& ) const;
	class Impl;
	RenderTarget( const Impl* );
private:
	friend class Manager;
	Impl* mImpl;
};

}
}

#endif