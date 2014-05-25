#include "Mashiro/Mashiro.h"
#include "Mashiro/Scene/AnimationModel.h"
#include "Mashiro/Graphics/Texture.h"
#include "Mashiro/Graphics/RenderTarget.h"
#include "Mashiro/Graphics/GraphicsManager.h"
#include "Mashiro/Graphics/src/GraphicsManagerImpl.h"
#include "Mashiro/Graphics/Enum.h"
#include "Mashiro/Math/Vector3.h"
#include "Mashiro/Math/Functions.h"
#include "Mashiro/Base/Impl/ReferenceType.h"
#include "Mashiro/Math/Matrix.h"

#include "AnimationModelImpl.h"

namespace Mashiro{

using namespace Mashiro::Graphics;

namespace Scene {

AnimationModel::AnimationModel( Impl* impl ) : mImpl( impl ){
	if( mImpl ){
		mImpl->refer();
	}
}

void AnimationModel::draw() const {
	ASSERT( mImpl && "Graphics::Model : This is empty Object" );
	mImpl->draw();
}

void AnimationModel::setAngle( const Vector3& angle ){
	ASSERT( mImpl && "Graphics::Model : This is empty Object" );
	mImpl->setAngle( angle );
}

void AnimationModel::setColor( const Vector3& color, int num ){
	ASSERT( mImpl && "Graphics::Model : This is empty Object" );
	mImpl->setColor( color, num );
}

void AnimationModel::setScale( const Vector3& scale ){
	ASSERT( mImpl && "Graphics::Model : This is empty Object" );
	mImpl->setScale( scale );
}

void AnimationModel::setPosition( const Vector3& position ){
	ASSERT( mImpl && "Graphics::Model : This is empty Object" );
	mImpl->setPosition( position );
}

void AnimationModel::setPosition( const Vector2& pos ){
	ASSERT( mImpl && "Graphics::Model : This is empty Object" );
	//mImpl->setPosition( pos, gManagerImpl->mShaderParameter.mProjection, gManagerImpl->mShaderParameter.mView );
}

void AnimationModel::setTexture( const Texture& tex, int num ){
	ASSERT( mImpl && "Graphics::Model : This is empty Object" );
	mImpl->setTexture( tex, num );
}

void AnimationModel::setTexture(const RenderTarget& targetTexture, int num /*= 0 */) {
	ASSERT( mImpl && "Graphics::Model : This is empty Object" );
	mImpl->setTexture( targetTexture, num );
}

void AnimationModel::setTransparaency( float c ){
	ASSERT( mImpl && "Graphics::Model : This is empty Object" );
	mImpl->setTransparaency( c );
}

void AnimationModel::setAnimation( const FBXSkinMeshLoader& fbx ){
	ASSERT( mImpl && "Graphics::Model : This is empty Object" );
	mImpl->setAnimation( fbx.mImpl );
}

void AnimationModel::setAnimation( const SINFileLoader& sin ){
	ASSERT( mImpl && "Graphics::Model : This is empty Object" );
	mImpl->setAnimation( sin.mImpl );
}

void AnimationModel::setStartAnimation( int t ){
	ASSERT( mImpl && "Graphics::Model : This is empty Object" );
	mImpl->setStartAnimation( t );
}

void AnimationModel::setStopAnimationTime( int t ){
	ASSERT( mImpl && "Graphics::Model : This is empty Object" );
	mImpl->setStopAnimation( t );
}

void AnimationModel::update(){
	ASSERT( mImpl && "Graphics::Model : This is empty Object" );
	mImpl->update();
}

void AnimationModel::draw( const Matrix& world ) const {
	ASSERT( mImpl && "Graphics::Model : This is empty Object" );
	mImpl->draw( world );
}

const Vector3* AnimationModel::angle() const {
	ASSERT( mImpl && "Graphics::Model : This is empty Object" );
	return &mImpl->mAngle;
}

const Vector3* AnimationModel::scale() const {
	ASSERT( mImpl && "Graphics::Model : This is empty Object" );
	return &mImpl->mScale;
}

const Vector3* AnimationModel::color() const {
	ASSERT( mImpl && "Graphics::Model : This is empty Object" );
	return &mImpl->mColor;
}

float AnimationModel::transparaency() const {
	ASSERT( mImpl && "Graphics::Model : This is empty Object" );
	return mImpl->mTrans;
}

const Vector3* AnimationModel::position() const {
	ASSERT( mImpl && "Graphics::Model : This is empty Object" );
	return &mImpl->mPosition;
}

#define TYPE AnimationModel
#include "Mashiro/Base/Impl/ReferenceTypeTemplate.h"

}
}