#include "Mashiro/Mashiro.h"
#include "Mashiro/Graphics/GraphicsManager.h"
#include "Mashiro/Scene/SINFileLoader.h"
#include "Mashiro/Scene/src/SINFileLoaderImpl.h"
#include "Mashiro/Scene/AnimationModel.h"
#include "Mashiro/Scene/src/AnimationModelImpl.h"

namespace Mashiro{

namespace Scene {

SINFileLoader SINFileLoader::create( const char* fileName ){
	SINFileLoader r;
	r.mImpl = NEW Impl( fileName );
	return r;
}

#define TYPE SINFileLoader
#include "Mashiro/Base/Impl/ReferenceTypeTemplate.h"

}
}