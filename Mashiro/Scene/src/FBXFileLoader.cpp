#include "Mashiro/Mashiro.h"
#include "Mashiro/Graphics/GraphicsManager.h"
#include "Mashiro/Scene/FBXFileLoader.h"
#include "Mashiro/Scene/src/FBXFileLoaderImpl.h"
#include "Mashiro/Scene/Model.h"
#include "Mashiro/Scene/src/ModelImpl.h"

namespace Mashiro{

namespace Scene {

FBXFileLoader FBXFileLoader::create(){
	FBXFileLoader r;
	r.mImpl = NEW Impl();
	return r;
}

Model FBXFileLoader::createModel( const char* fileName ){
	ASSERT( mImpl && "Graphics::XFileLoader : This is empty Object" );
	mImpl->createModel( fileName );
	Model::Impl* modelImpl = NEW Model::Impl( mImpl );
	modelImpl->release();
	return Model( modelImpl );
	return Model();
}

#define TYPE FBXFileLoader
#include "Mashiro/Base/Impl/ReferenceTypeTemplate.h"

}
}