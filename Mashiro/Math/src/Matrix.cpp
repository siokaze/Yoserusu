#include "Mashiro/Mashiro.h"
#include "Mashiro/Math/Matrix.h"
#include "Mashiro/Math/Vector3.h"
#include "Mashiro/Math/Vector4.h"
#include "Mashiro/Math/Functions.h"
#include <D3DX10math.h>

namespace Mashiro{
namespace Math{

Matrix::Data::Data(){
	m11 = m12 = m13 = m14 = 0.f;
	m21 = m22 = m23 = m24 = 0.f;
	m31 = m32 = m33 = m34 = 0.f;
	m41 = m42 = m43 = m44 = 0.f;
}

void Matrix::Data::set( const float* f ){
	m11 = f[ 0 ];  m12 = f[ 1 ];  m13 = f[ 2 ];  m14 = f[ 3 ];
	m21 = f[ 4 ];  m22 = f[ 5 ];  m23 = f[ 6 ];  m24 = f[ 7 ];
	m31 = f[ 8 ];  m32 = f[ 9 ];  m33 = f[ 10 ]; m34 = f[ 11 ];
	m41 = f[ 12 ]; m42 = f[ 13 ]; m43 = f[ 14 ]; m44 = f[ 15 ];
}

Matrix::Matrix(){
}

void Matrix::setIdentity(){
	m.m11 = m.m22 = m.m33 = m.m44 = 1.f;
	
	m.m12 = m.m13 = m.m14 = 0.f;
	m.m21 = m.m23 = m.m24 = 0.f;
	m.m31 = m.m32 = m.m34 = 0.f;
	m.m41 = m.m42 = m.m43 = 0.f;

}

void Matrix::setPerspectiveFovLH(
float fieldOfViewY,
float windowWidth,
float windowHeight,
float nearClip,
float farClip ){
	D3DXMATRIX a;
	D3DXMatrixPerspectiveFovLH( 
		&a,
		D3DXToRadian( fieldOfViewY ),
		static_cast< FLOAT >( windowWidth / windowHeight ),
		static_cast< FLOAT >( nearClip ), 
		static_cast< FLOAT >( farClip ) );
	m.set( a );
}

void Matrix::setOrthoLH(
float width,
float height,
float nearClip,
float farClip ){
	D3DXMATRIX a;
	D3DXMatrixOrthoLH( &a, width, height, nearClip, farClip );
	a._22 *= -1.0f;
	a._41 = -1.0f;
	a._42 = 1.0f;
	m.set( a );
}

void Matrix::setLookAtLH(
const Vector3& p,
const Vector3& t,
const Vector3& u ){
	D3DXMATRIX a;
	D3DXMatrixLookAtLH( 
		&a,
		&D3DXVECTOR3(p.x, p.y, p.z ),
		&D3DXVECTOR3(t.x, t.y, t.z ),
		&D3DXVECTOR3(u.x, u.y, u.z ) );
	m.set( a );
}

Vector3 Matrix::convertWorld(
int width,
int height,
Vector3* screenVec,
Matrix projectionMatrix,
Matrix viewMatrix ) {
	//出力ベクトル
	D3DXVECTOR3 out( 0, 0, 0 );

	//Vector3に変換
	D3DXVECTOR3 local( screenVec->x, screenVec->y, 0 );

	//ビューボート行列の作成
	D3D10_VIEWPORT viewPort2;
	viewPort2.Height = height;
	viewPort2.Width = width;
	viewPort2.TopLeftX = 0;
	viewPort2.TopLeftY = 0;
	viewPort2.MaxDepth = 1.f;
	viewPort2.MinDepth = 0.f;


	D3DXMATRIX viewPort, mat, invMat;
	D3DXMatrixIdentity( &mat );
	D3DXMatrixInverse( &invMat,NULL, &mat );
	D3DXMatrixIdentity( &viewPort );

	viewPort._11 = static_cast< float >(  width / 2 );
	viewPort._41 = static_cast< float >(  width / 2 );
	viewPort._22 = static_cast< float >( -height / 2 );
	viewPort._42 = static_cast< float >( height / 2 );
	viewPort._33 = 1.0f;
	viewPort._44 = 1.0f;

	D3DXMATRIX view(&viewMatrix.m.m11), proj( &projectionMatrix.m.m11 );
	D3DXMATRIX invView, invProj, invViewProt;
	D3DXMatrixInverse( &invViewProt, NULL, &viewPort );
	D3DXMatrixInverse( &invProj, NULL, &proj );
	D3DXMatrixInverse( &invView, NULL, &view );

	float f1 = ( 1.f / width );
	float f2 = ( 1.f / height );

	//	スクリーン上の座標を行列に。行列で計算するため念のため
	D3DXMATRIX screenPos;
	screenPos._11 = screenPos._21 = screenPos._31 = screenPos._41 = local.x; 
	screenPos._12 = screenPos._22 = screenPos._32 = screenPos._42 = local.y;
	screenPos._13 = screenPos._23 = screenPos._33 = screenPos._43 = 0.9f;		//	ここをなんとかする
	screenPos._14 = screenPos._24 = screenPos._34 = screenPos._44 = 1;

	//	ビューポート→プロジェクション→ビュー→ワールドと変換を行う
	D3DXMATRIX ret = screenPos * invViewProt * invProj * invView * mat;

	//	変換した値を取得
	out.x = ret._11 / ret._14;
	out.y = ret._12 / ret._14;
	out.z = ret._13 / ret._14;

	return Vector3( out.x, out.y, out.z );
}

void Matrix::bilBoard( const Vector3& billPos, const Vector3& targetPos ){
	D3DXMATRIX a;
	D3DXMatrixIdentity( &a );
	D3DXMatrixLookAtLH( 
		&a, 
		&D3DXVECTOR3( targetPos.x, targetPos.y, targetPos.z ), 
		&D3DXVECTOR3( billPos.x, billPos.y, billPos.z ),
		&D3DXVECTOR3( 0, 1, 0 ) );
	a._41 = a._42 = a._43 = 0;
	m.set( a );
}

void Matrix::setRotationZ( float rotate ){
	D3DXMATRIX a;
	D3DXMatrixRotationZ( &a, D3DXToRadian( static_cast< FLOAT >( rotate ) ) );
	m.set( a );
}

void Matrix::setRotationX( float rotate ){
	D3DXMATRIX a;
	D3DXMatrixRotationX( &a, D3DXToRadian( static_cast< FLOAT >( rotate ) ) );
	m.set( a );
}

void Matrix::setRotationY( float rotate ){
	D3DXMATRIX a;
	D3DXMatrixRotationY( &a, D3DXToRadian( static_cast< FLOAT >( rotate ) ) );
	m.set( a );
}

void Matrix::setTranslation( const Vector3& p ){
	D3DXMATRIX a;
	D3DXMatrixTranslation( &a,  p.x, p.y, p.z );
	m.set( a );
}

void Matrix::setTranslation( float x, float y, float z ){
	D3DXMATRIX a;
	D3DXMatrixTranslation( &a,  x, y, z );
	m.set( a );
}

Vector4 Matrix::translation( const Vector4& vec, const Matrix& mat ){
	D3DXVECTOR4 v( vec.x, vec.y, vec.z, vec.w );
	D3DXVECTOR4 out;
	D3DXVec4Transform( &out, &v, &D3DXMATRIX( &mat.m.m11 ) );
	return Vector4( out.x, out.y, out.z, out.w );
}

void Matrix::mul( Vector3* out, const Vector3& in ) const {
	float tx = in.x;
	float ty = in.y;
	out->x = m.m11 * tx + m.m12 * ty + m.m13 * in.z + m.m14;
	out->y = m.m21 * tx + m.m22 * ty + m.m23 * in.z + m.m24;
	out->z = m.m31 * tx + m.m32 * ty + m.m33 * in.z + m.m34;
}

void Matrix::setMul( const Matrix& a, const Matrix& b ){
	D3DXMATRIX mul;
	D3DXMatrixMultiply( &mul, &D3DXMATRIX((float*)&a.m), &D3DXMATRIX((float*)&b.m) );
	m.set( mul );
}

void Matrix::setScaling( const Vector3& p ){
	D3DXMATRIX a;
	D3DXMatrixScaling( &a, p.x, p.y, p.z );
	m.set( a );
}

void Matrix::setScaling( float f ){
	D3DXMATRIX a;
	D3DXMatrixScaling( &a, f, f, f );
	m.set( a );
}

void Matrix::setScaling( float x, float y, float z ){
	D3DXMATRIX a;
	D3DXMatrixScaling( &a, x, y, z );
	m.set( a );
}

void Matrix::setTranspose( const Matrix& mat ){
	D3DXMATRIX a;
	D3DXMatrixTranspose( &a, &D3DXMATRIX((float*)&mat.m ) );
	m.set( a );
}

void Matrix::setInverse( const Matrix& mat ){
	D3DXMATRIX a;
	D3DXMatrixInverse( &a, NULL, &D3DXMATRIX((float*)&mat.m ) );
	m.set( a );
}

float& Matrix::operator()( int r, int c ){
	ASSERT( r >= 0 && r < 4 );
	ASSERT( c >= 0 && c < 4 );
	return ( &m.m11 )[ 4 * r + c ];
}

float Matrix::operator()( int r, int c ) const {
	ASSERT( r >= 0 && r < 4 );
	ASSERT( c >= 0 && c < 4 );
	return ( &m.m11 )[ 4 * r + c ];
}

bool Matrix::operator==( const Matrix& a ) const {
	for ( int i = 0; i < 4; ++i ){
		for ( int j = 0; j < 4; ++j ){
			if ( ( *this )( i, j ) != a( i, j ) ){
				return false;
			}
		}
	}
	return true;
}

bool Matrix::operator!=( const Matrix& a ) const {
	return !( operator==( a ) );
}

Matrix Matrix::operator*( const Matrix& in ) {
	Matrix out;
	D3DXMATRIX t0( &out.m.m11 );
	D3DXMATRIX t1( &in.m.m11 );

	D3DXMATRIX ans = t0 * t1;
	
	out.m.set( ans );

	return out;
}

void Matrix::operator*=( const Matrix& in ){
	D3DXMATRIX t0( &m.m11 );
	D3DXMATRIX t1( &in.m.m11 );
	
	t0 *= t1;

	m.set( t0 );
}

}
}