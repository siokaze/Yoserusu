#include "Score.h"

Score* Score::mInstance = 0;

void Score::create(){ 
	ASSERT( !mInstance);
	mInstance = new Score();
}

void Score::destory(){
	ASSERT(mInstance );
	SAFE_DELETE( mInstance );
}

Score* Score::instance(){ 
	return mInstance;
}

Score::Score() : mCount( 0 ){
}

Score::~Score(){

}

void Score::Add(){
	++mCount;
}

int Score::getScore() const {
	return mCount;
}

void Score::Init(){
	mCount = 0;
}