#ifndef _ARM_H__
#define _ARM_H__

#include "Mashiro/Math/Vector2.h"
#include "Mashiro/Math/Vector3.h"
#include "Mashiro/Scene/Model.h"

using namespace Mashiro::Graphics;
using namespace Mashiro::Scene;
using namespace Mashiro::Math;

class Arm{
public:
	Arm(){ keep = false; depth = 0;}
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
		Model model;
		Vector3 angle;
	};
	Joint shoulder;
	Joint elbow;
	Joint wrist;
	int depth;
	bool keep;
};

#endif	// end of Arm