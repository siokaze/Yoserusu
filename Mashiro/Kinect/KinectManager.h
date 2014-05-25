#ifndef INCLUDE_MASHIRO_KINECT_H_
#define INCLUDE_MASHIRO_KINECT_H_

#include "Mashiro/Kinect/KinectEnum.h"

namespace Mashiro {
	namespace Graphics {
		class Texture;
	}
	namespace Math {
		class Vector2;
	}

namespace Kinect {

class Manager {
public:
	Manager();
	static Manager instance();

	//RBGカラー
	Graphics::Texture colorTexture() const;
	//深度情報
	Graphics::Texture depthTexture() const;
	//スケルトン情報
	Math::Vector2 skeletonPos( int i ) const;
	//これどのスケルトン？
	Math::Vector2 index( SkeletonIndex ) const;
	//トラッキングしてる？
	bool tracking() const;
	//スケルトンの深度値
	int depthSkeleton( int i ) const;
	//トラッキング固定フラグ
	void initTrackingFlag();

	//以下は知らなくてよい
	void update();
	static void create( int w, int h, bool window );
	static void destroy();
};

}
}

#endif
