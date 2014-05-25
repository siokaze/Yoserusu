#ifndef INCLUDE_MASHIRO_MATH_MATRIX_44_H_
#define INCLUDE_MASHIRO_MATH_MATRIX_44_H_

//#include <D3DX10math.h>

namespace Mashiro{

namespace Math{

class Vector3;
class Vector4;

class Matrix{
public:
	Matrix();
	//単位行列
	void setIdentity();
	//透視変換行列の生成
	void setPerspectiveFovLH( 
		float fieldOfViewYInRadian,
		float windowWidth,
		float windowHeight,
		float nearClip,
		float farClip );
	//正射影行列の生成
	void setOrthoLH(
		float width,
		float height,
		float nearClip,
		float farClip );
	//view行列を作ってその場で掛け算する
	void setLookAtLH(
		const Vector3& eyePosition,
		const Vector3& eyeTarget,
		const Vector3& upVector );
	//スクリーン座標を３D空間へ
	Vector3 convertWorld(
		int width, int height,
		Vector3* vec,
		Matrix projectionMatrix,
		Matrix viewMatrix );
	//ビルボード
	void bilBoard(
		const Vector3& bilPos,
		const Vector3& targetPos );

	//移動
	void setTranslation( float, float, float );
	void setTranslation( const Vector3& );
	Math::Vector4 translation( const Vector4&, const Matrix& );
	//回転
	void setRotationX( float );
	void setRotationY( float );
	void setRotationZ( float );
	//拡大縮小
	void setScaling( float xyz );
	void setScaling( float x, float y, float z );
	void setScaling( const Vector3& );
	//逆行列
	void setInverse( const Matrix& );
	//転置行列
	void setTranspose( const Matrix& );

	void setMul( const Matrix&, const Matrix& );
	void mul( Vector3* out, const Vector3& in ) const;
	///完全に一致しているかチェック
	bool operator==( const Matrix& ) const;
	///少しでも違うのかをチェック
	bool operator!=( const Matrix& ) const;
	Matrix operator*( const Matrix& );
	void operator*=( const Matrix& );
	float& operator()( int row, int col );
	float operator()( int row, int col ) const;

	//D3DXMATRIX m;
	struct Data {
		Data();
		void set( const float* fp );
		float m11, m12, m13, m14;
		float m21, m22, m23, m24;
		float m31, m32, m33, m34;
		float m41, m42, m43, m44;
	};
	Data m;
};

}
}

#endif
