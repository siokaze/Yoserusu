#include "Mashiro/Mashiro.h"
#include "Mashiro/Graphics/Shader.h"
#include "Mashiro/Graphics/src/ShaderImpl.h"
#include "Mashiro/Graphics/src/GraphicsManagerImpl.h"

namespace Mashiro{
namespace Graphics{

Shader Shader::create( ShaderType type ){
	Shader r;
	r.mImpl = NEW Impl( type, gManagerImpl->dxDevice() );
	return r;
}

bool Shader::lock( void** buffer ){
	ASSERT( mImpl && "Graphics::IndexBuffer : This is empty object." );
	return mImpl->lock( buffer );
}

void Shader::unlock(){
	ASSERT( mImpl && "Graphics::IndexBuffer : This is empty object." );
	mImpl->unlock();
}

#define TYPE Shader
#include "Mashiro/Base/Impl/ReferenceTypeTemplate.h"

}
}