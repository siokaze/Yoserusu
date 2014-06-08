#include "ModelObj.h"
#include "ModelLoader.h"
#include "Mashiro/Graphics/GraphicsManager.h"

using namespace Mashiro;
using namespace Mashiro::Math;
using namespace Mashiro::Graphics;
using namespace Mashiro::Scene;

void ModelObj::create(const char* fileName) {
	mModel = ModelLoader::instance()->createModel( fileName );
	mCocTrans = std::make_shared< CocTrans >();
}

void ModelObj::setColor(const Vector3& color) {
	mModel.setColor( color );
}

void ModelObj::setTrance(float t) {
	mModel.setTransparaency( t );
}

void ModelObj::draw( CocTrans::Type type ){
	//Še•”ˆÊ‚Ìƒ[ƒ‹ƒhÀ•W•ÏŠ·iŠgkA‰ñ“]A•½sˆÚ“®j
	Matrix world, scale, rotateZ, rotateY, rotateX, trans;
	//Šg‘åk¬
	scale.setScaling( mScale );
	//‰ñ“]
	rotateZ.setRotationZ( mRotate.z );
	rotateX.setRotationX( mRotate.x );
	rotateY.setRotationY( mRotate.y );
	//ˆÚ“®
	trans.setTranslation( mPosition );

	//Š|‚¯ŽZ
	world.setIdentity();
	world.setMul(world, scale);
	world.setMul(world, rotateZ);
	world.setMul(world, rotateX);
	world.setMul(world, rotateY);
	world.setMul(world, trans);

	CocTrans::ConstantBuffer* cb = NULL;
	if( mCocTrans->lock( (void**)&cb ) ){
		cb->mDrawType = type;
		mCocTrans->unLock();
	}
	cb = nullptr;
	mCocTrans->worldLight( world );	
	mCocTrans->setShader();

	mModel.draw( world );
}

void ModelObj::setTexture(const Mashiro::Graphics::Texture& tex, int stage) {
	if( stage == 0 ){
		mModel.setTexture( tex );
	} else if( stage >= 0 ){
		Graphics::Manager::instance().setTexture( tex, stage );
	}
}

ModelObj::ModelObj() {

}

ModelObj::ModelObj(const char* fileName) {
	create( fileName );
}

