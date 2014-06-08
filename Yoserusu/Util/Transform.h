#ifndef INCLUDE_TRANSFORM_H_
#define INCLUDE_TRANSFORM_H_

#include "Mashiro/Math/Vector3.h"
#include "Mashiro/Math/Matrix.h"

using namespace Mashiro::Math;

class Transform {
public:
	Transform() : mPosition(), mRotate(), mScale( 1.f ){}

	void setPosition( const Vector3& pos ){ mPosition = pos; }
	void setAngle( const Vector3& rotate ){ mRotate = rotate; }
	void setScale( const Vector3& scale ){ mScale = scale; }

protected:
	Vector3 mPosition;
	Vector3 mRotate;
	Vector3 mScale;
};


#endif