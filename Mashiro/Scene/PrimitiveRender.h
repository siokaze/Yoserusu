#ifndef INCLUDE_MASHIRO_SCENE_PRIMITIVE_RENDER_H_
#define INCLUDE_MASHIRO_SCENE_PRIMITIVE_RENDER_H_

namespace Mashiro{

namespace Math{
class Vector2;
class Vector3;
class Vector4;
class Matrix;
}

namespace Graphics{
class Bitmap;
}

namespace Scene {

using namespace Math;
using namespace Graphics;

class PrimitiveRender {
public:
	static PrimitiveRender create();

	void setPosition( const Vector2& );
	void setAngle( float );
	void setScale( const Vector3& );
	void setColor( const Vector3& );
	void setTransparaency( float );
	void setTexture( const Bitmap& bitmap );

	//•`‰æ
	void draw() const;

	PrimitiveRender();
	~PrimitiveRender();
	PrimitiveRender( const PrimitiveRender& );
	operator void*() const;
	PrimitiveRender& operator=( const PrimitiveRender& );
	void release();
	bool operator==( const PrimitiveRender& ) const;
	bool operator!=( const PrimitiveRender& ) const;

	class Impl;
	explicit PrimitiveRender( Impl* );
private:
	Impl* mImpl;
};

}
}

#endif