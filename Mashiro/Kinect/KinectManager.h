#ifndef INCLUDE_MASHIRO_KINECT_H_
#define INCLUDE_MASHIRO_KINECT_H_

#include "Mashiro/Kinect/KinectEnum.h"

namespace Mashiro {
	namespace Graphics {
		class Bitmap;
	}
	namespace Math {
		class Vector2;
	}

namespace Kinect {

class Manager {
public:
	Manager();
	static Manager instance();

	//RBG�J���[
	Graphics::Bitmap colorTexture() const;
	//�[�x���
	Graphics::Bitmap depthTexture() const;
	//�X�P���g�����
	Math::Vector2 skeletonPos( int i ) const;
	//����ǂ̃X�P���g���H
	Math::Vector2 index( SkeletonIndex ) const;
	//�g���b�L���O���Ă�H
	bool tracking() const;
	//�X�P���g���̐[�x�l
	int depthSkeleton( int i ) const;
	//�g���b�L���O�Œ�t���O
	void initTrackingFlag();

	//�ȉ��͒m��Ȃ��Ă悢
	void update();
	static void create( int w, int h, bool window );
	static void destroy();
};

}
}

#endif
