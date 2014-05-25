#ifndef INCLUDE_MASHIRO_GRAPHICS_ANIMATION_MODEL_H_
#define INCLUDE_MASHIRO_GRAPHICS_ANIMATION_MODEL_H_

namespace Mashiro{

namespace Math{
	class Vector2;
	class Vector3;
	class Vector4;
	class Matrix;
	
}

namespace Graphics{
	class Texture;
	class RenderTarget;
}

namespace Scene {

class FBXSkinMeshLoader;
class MRSFileLoader;
class SINFileLoader;

using namespace Math;
using namespace Graphics;

class AnimationModel {
public:
	//•`‰æ
	void draw() const;
	void draw( const Matrix& world ) const;

	void update();
	
	const Vector3* position() const;
	const Vector3* angle() const;
	const Vector3* scale() const;
	const Vector3* color() const;
	float transparaency() const;

	void setPosition( const Vector3& );
	void setPosition( const Vector2& in );
	void setAngle( const Vector3& );
	void setScale( const Vector3& );
	void setColor( const Vector3&, int num = -1 );
	void setTransparaency( float );
	void setTexture( const RenderTarget& targetTexture, int num = -1 );
	void setTexture( const Texture& tex, int num = -1 );
	void setAnimation( const FBXSkinMeshLoader& );
	void setAnimation( const SINFileLoader& );
	void setStartAnimation( int time );
	void setStopAnimationTime( int time );

	AnimationModel();
	~AnimationModel();
	AnimationModel( const AnimationModel& );
	operator void*() const;
	AnimationModel& operator=( const AnimationModel& );
	void release();
	bool operator==( const AnimationModel& ) const;
	bool operator!=( const AnimationModel& ) const;

	class Impl;
	explicit AnimationModel( Impl* );
private:
	Impl* mImpl;

};


}
}

#endif
