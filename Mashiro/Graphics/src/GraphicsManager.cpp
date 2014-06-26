#include "Mashiro/Mashiro.h"
#include "Mashiro/Graphics/GraphicsManager.h"
#include "Mashiro/Graphics/src/GraphicsManagerImpl.h"
#include "Mashiro/Window/Window.h"

namespace Mashiro{
namespace Graphics {

GraphicsManagerImpl* gManagerImpl = 0;

Manager::Manager(){
	//•Ê‚ÌƒXƒŒƒbƒhNG
	ASSERT( Window::isMainThread() && "you must call from MAIN thread" );
}

void Manager::create(
void* hwnd,
int w,
int h,
bool windowScreen ){
	STRONG_ASSERT( !gManagerImpl );
	gManagerImpl = NEW GraphicsManagerImpl(
		static_cast< HWND >( hwnd ),
		w,
		h,
		windowScreen );
}

void Manager::destroy(){
	SAFE_DELETE( gManagerImpl );
}

Manager Manager::instance(){
	return Manager();
}

void Manager::enableDepthTest( bool f ){
	gManagerImpl->setDepthTest( f );
}

void Manager::enableDepthWrite( bool f ){
	gManagerImpl->setDepthWrite( f );
}

void Manager::draw( PrimitiveType type ) {
	gManagerImpl->draw( type );
}

void Manager::drawIndexed( int indexCount, int start, PrimitiveType type ) {
	gManagerImpl->drawIndexed( indexCount, start, type );
}
void Manager::setVertexBuffer( VertexBuffer v ){
	gManagerImpl->setVertexBuffer( v.mImpl );
}

void Manager::setIndexBuffer( IndexBuffer i ){
	gManagerImpl->setIndexBuffer( i.mImpl );
}

void Manager::setTexture( Texture t, int stage ){
	gManagerImpl->setTexture( t.mImpl, stage );
}

void Manager::setTexture(RenderTarget targetTexture, int stage) {
	gManagerImpl->setTextureRender( targetTexture.mImpl, stage );
}

void Manager::setShader( Shader shader ){
	gManagerImpl->setShader( shader.mImpl );
}

void Manager::end(){
	gManagerImpl->end();
}

int Manager::height() const {
	return gManagerImpl->mHeight;
}

int Manager::width() const {
	return gManagerImpl->mWidth;
}

void Manager::getPointerModifier( float* scale, Vector2* offset ) const {
	gManagerImpl->getPointerModifier( scale, offset );
}

void Manager::setViewPort( int x, int y, int w, int h ){
	gManagerImpl->setViewport( x, y, w, h );
}

void Manager::getViewPort( int* x, int* y, int* w, int* h ){
	gManagerImpl->getViewport( x, y, w, h );
}

void Manager::setCullMode( CullMode c ){
	gManagerImpl->setCullMode( c );
}

void Manager::setBlendMode( BlendMode sorce){
	gManagerImpl->setBlendMode( sorce );
}

void Manager::enableAlphaBlend( bool r ){
	gManagerImpl->enableAlphaBlend( r );
}

void Manager::enableDepthWriteMask( bool r ){
	gManagerImpl->enableDepthWriteMask( r );
}

void Manager::setWorldMatrix(const Math::Matrix& mat)
{
	gManagerImpl->setWorldMatrix( mat );
}

void Manager::setProjectionMatrix(const Math::Matrix& mat)
{
	gManagerImpl->setProjectionMatrix( mat );
}

void Manager::setViewMatrix(const Math::Matrix& mat)
{
	gManagerImpl->setViewMatrix( mat );
}

void Manager::setLight(const Math::Vector4& light)
{
	gManagerImpl->setLight( light );
}

void Manager::setEyePos(const Math::Vector4& eye)
{
	gManagerImpl->setEyePos( eye );
}

void Manager::setDiffuse(const Math::Vector4& diffuse)
{
	gManagerImpl->setDiffuse( diffuse );
}

void Manager::setAmbient(const Math::Vector4& amb )
{
	gManagerImpl->setAmbient( amb );
}

void Manager::setSpecular(const Math::Vector4& spc)
{
	gManagerImpl->setSpecular( spc );
}

void Manager::setRenderTarget(int num, RenderTarget target[])
{
	RenderTarget::Impl* tvtImpl[ 8 ];
	for( int i = 0; i < num; ++i ){
		tvtImpl[ i ] = target[ i ].mImpl;
	}
	gManagerImpl->setRenderTarget( num, tvtImpl );
}

void Manager::setBackBufferTarget()
{
	gManagerImpl->setBackBufferTarget();
}

Matrix Manager::viewMatrix() const {
	return gManagerImpl->mBasicConstantBuffer.mView;
}

}
}
