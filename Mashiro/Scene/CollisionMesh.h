#ifndef INCLUDE_MASHIRO_COLLISIONDETECTOR_H_
#define INCLUDE_MASHIRO_COLLISIONDETECTOR_H_

namespace Mashiro{
namespace Math {
	class Vector3;
};

namespace Scene {
using namespace Math;

class PMDFileLoader;

class CollisionMesh {
public:
	//PMDファイルからコンストラクト
	static CollisionMesh create( const PMDFileLoader&, const Vector3& vec );
	///ロード終了検知。ファイル名から作った場合はtrueが返ることを確認してから使うこと。
	bool isReady();
	///線分と交差判定。
	bool isIntersect( const Vector3& lineOrigin, const Vector3& lineVector ) const;
	///線分と交差判定。最初に当たった時刻をtimeOutに入れる。当たればtrueを返す。
	bool getIntersection( 
		float* timeOut, //0ポインタなら返さない
		Vector3* normalOut, //0ポインタなら返さない
		const Vector3& lineOrigin, 
		const Vector3& lineVector ) const;

	CollisionMesh();
	~CollisionMesh();
	CollisionMesh( const CollisionMesh& );
	operator void*() const;
	CollisionMesh& operator=( const CollisionMesh& );
	void release();
	bool operator==( const CollisionMesh& ) const;
	bool operator!=( const CollisionMesh& ) const;
private:
	class Impl;
	explicit CollisionMesh( Impl* );
	Impl* mImpl;

};

}
}

#endif