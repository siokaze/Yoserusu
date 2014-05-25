#include "Mashiro/Mashiro.h"
#include "Mashiro/Graphics/GraphicsManager.h"
#include "Mashiro/Scene/PMDFileLoader.h"
#include "Mashiro/Scene/src/PMDFileLoaderImpl.h"
#include "Mashiro/Scene/Model.h"
#include "Mashiro/Scene/src/ModelImpl.h"

namespace Mashiro{

namespace Scene {

PMDFileLoader PMDFileLoader::create(){
	PMDFileLoader r;
	r.mImpl = NEW Impl();
	return r;
}

Model PMDFileLoader::createModel( const char* fileName ){
	ASSERT( mImpl && "Graphics::XFileLoader : This is empty Object" );
	mImpl->createModel( fileName );
	Model::Impl* modelImpl = NEW Model::Impl( mImpl );
	modelImpl->release();
	return Model( modelImpl );
}

#define TYPE PMDFileLoader
#include "Mashiro/Base/Impl/ReferenceTypeTemplate.h"

}
}