#ifndef INCLUDED_MASHIRO_BASE_REFERENCETYPE_H
#define INCLUDED_MASHIRO_BASE_REFERENCETYPE_H

//参照カウントを持つ型を作る時に楽にするための基底クラス
namespace Mashiro{

class ReferenceType{
public:
	ReferenceType() : mReferenceCount( 1 ){
	}
	void refer(){
		++mReferenceCount;
	}
	void release(){
		--mReferenceCount;
	}
	int referenceCount() const {
		return mReferenceCount;
	}
protected:
	~ReferenceType(){
	}
private:
	int mReferenceCount;
};

} //namespace Mashiro

#endif
