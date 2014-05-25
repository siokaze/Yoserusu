#include "Mashiro/Mashiro.h"
#include "Mashiro/Graphics/RenderTarget.h"
#include "Mashiro/Graphics/src/RenderTargetImpl.h"
#include "Mashiro/Graphics/src/GraphicsManagerImpl.h"

namespace Mashiro{
namespace Graphics{

RenderTarget RenderTarget::create( int w, int h, RenderFormat textureFormat ){
	RenderTarget r;
	r.mImpl = NEW Impl( w, h, textureFormat, gManagerImpl->dxDevice() );
	return r;
}

void RenderTarget::targetTexture(const char* name) {
	ASSERT( mImpl && "Graphics::VertexBuffer : This is empty Object" );
	mImpl->targetTexture( name );
}


#define TYPE RenderTarget
#include "Mashiro/Base/Impl/ReferenceTypeTemplate.h"

}
}