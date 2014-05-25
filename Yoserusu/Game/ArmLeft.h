#ifndef ARMLEFT_H
#define ARMLEFT_H

#include "Mashiro/Math/Matrix.h"

#include "Arm.h"

using namespace Mashiro;
namespace Mashiro{
	
	class ArmLeft : public Arm{
	public:
		ArmLeft();
		~ArmLeft();
		void Update(float depth,int number);
		void Draw();

		Vector2 Pos();

	private:
		Matrix sworld;
		Matrix eworld;
		Matrix hworld;
	};
}
#endif
