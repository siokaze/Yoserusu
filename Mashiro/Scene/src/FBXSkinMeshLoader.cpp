#include "Mashiro/Mashiro.h"
#include "Mashiro/Graphics/GraphicsManager.h"
#include "Mashiro/Scene/FBXSkinMeshLoader.h"
#include "Mashiro/Scene/src/FBXSkinMeshLoaderImpl.h"
#include "Mashiro/Scene/AnimationModel.h"
#include "Mashiro/Scene/src/AnimationModelImpl.h"

namespace Mashiro{

namespace Scene {

FBXSkinMeshLoader FBXSkinMeshLoader::create(){
	FBXSkinMeshLoader r;
	r.mImpl = NEW Impl();
	return r;
}

AnimationModel FBXSkinMeshLoader::createModel( const char* fileName ){
	ASSERT( mImpl && "Graphics::XFileLoader : This is empty Object" );
	mImpl->createModel( fileName );
	AnimationModel::Impl* modelImpl = NEW AnimationModel::Impl( mImpl );
	modelImpl->release();
	return AnimationModel( modelImpl );
}

void FBXSkinMeshLoader::createMRSModelFile( const char* fileName ){
	ASSERT( mImpl && "Graphics::Model : This is empty Object" );
	return mImpl->createFile( fileName );
}

void FBXSkinMeshLoader::createSINAnimationFile( const char* fileName, int frame1, int frame2 ){
	ASSERT( mImpl && "Graphics::Model : This is empty Object" );
	return mImpl->createAnimationFile( fileName, frame1, frame2 );
}

#define TYPE FBXSkinMeshLoader
#include "Mashiro/Base/Impl/ReferenceTypeTemplate.h"

}
}