#include "Mashiro/Mashiro.h"
#include "Mashiro/Graphics/GraphicsManager.h"
#include "Mashiro/Scene/MRSFileLoader.h"
#include "Mashiro/Scene/src/MRSFileLoaderImpl.h"
#include "Mashiro/Scene/AnimationModel.h"
#include "Mashiro/Scene/Model.h"
#include "Mashiro/Scene/src/ModelImpl.h"
#include "Mashiro/Scene/src/AnimationModelImpl.h"

namespace Mashiro{

namespace Scene {

MRSFileLoader MRSFileLoader::create(){
	MRSFileLoader r;
	r.mImpl = NEW Impl();
	return r;
}

AnimationModel MRSFileLoader::createModel( const char* fileName ){
	ASSERT( mImpl && "Graphics::XFileLoader : This is empty Object" );
	mImpl->createModel( fileName );
	AnimationModel::Impl* modelImpl = NEW AnimationModel::Impl( mImpl );
	modelImpl->release();
	return AnimationModel( modelImpl );
}

Model MRSFileLoader::createStaticModel( const char* filename ){
	ASSERT( mImpl && "Graphics::XFileLoader : This is empty Object" );
	mImpl->static_createModel( filename );
	Model::Impl* modelImpl = NEW Model::Impl( mImpl );
	modelImpl->release();
	return Model( modelImpl );
}

#define TYPE MRSFileLoader
#include "Mashiro/Base/Impl/ReferenceTypeTemplate.h"

}
}