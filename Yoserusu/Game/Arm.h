#ifndef _ARM_H__
#define _ARM_H__

#include "Mashiro/Math/Vector2.h"
#include "Mashiro/Math/Vector3.h"
#include "Mashiro/Scene/Model.h"
#include "Util/TextureObj.h"
#include "Util/ModelObj.h"

using namespace Mashiro::Graphics;
using namespace Mashiro::Scene;
using namespace Mashiro::Math;

class Arm{
public:
	Arm(){ 
		mCubeTex.create( "res/image/uffizi_cross.dds" );
		keep = false; depth = 0;}
	virtual ~Arm(){}

	virtual void Update(float depth,int number) = 0;
	virtual void Draw() = 0;
	virtual Vector2 Pos() = 0;

	virtual void Init(){ keep = false;}
	virtual int Depth(){return depth;}
	virtual void Depth(int _depth){depth = _depth;}
	bool Keep(){ return keep;}
	void Keep(bool _keep){keep = _keep;}

protected:
	struct Joint{
		ModelObj model;
		Vector3 angle;
	};
	Joint shoulder;
	Joint elbow;
	Joint wrist;
	int depth;
	bool keep;

	TextureObj mCubeTex;
};

#endif	// end of Arm