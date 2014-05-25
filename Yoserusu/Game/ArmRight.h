#ifndef ARMRIGHT_H
#define ARMRIGHT_H

#include "Mashiro/Math/Matrix.h"
#include "Arm.h"

using namespace Mashiro;
namespace Mashiro{

	class ArmRight : public Arm{
	public:
		ArmRight();
		~ArmRight();
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
