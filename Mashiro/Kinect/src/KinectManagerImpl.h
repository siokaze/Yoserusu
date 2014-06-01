#ifndef INCLUDE_MASHIRO_KINECT_MANAGER_IMPL_H_
#define INCLUDE_MASHIRO_KINECT_MANAGER_IMPL_H_

#include <Windows.h>
#include <NuiApi.h>
#include <stdio.h>

#include "Mashiro/Mashiro.h"
#include "Mashiro/Kinect/KinectManager.h"
#include "Mashiro/Math/Vector2.h"
#include "Mashiro/Graphics/src/TextureImpl.h"
#include "Mashiro/Threading/Event.h"
#include "Mashiro/Graphics/Bitmap.h"
#include "Mashiro/Graphics/src/BitmapImpl.h"

using namespace Mashiro;
using namespace Mashiro::Graphics;

namespace Mashiro {

namespace Kinect {

class KinectManagerImpl {
public:
	KinectManagerImpl( int w, int h ) : 
	mNuiSensor( 0 ),
	mWidth( w ),
	mHeight( h ),
	mDepthPixels( 0 ),
	mTrackingFlag( false ),
	mUserIndex( -1 ),
	mTrackingID( -1 ){
		HRESULT hr;
		createInstance();
		if( NULL != mNuiSensor ){
			DWORD flag =  NUI_INITIALIZE_FLAG_USES_DEPTH | NUI_INITIALIZE_FLAG_USES_SKELETON | NUI_INITIALIZE_FLAG_USES_COLOR;
			if( SUCCEEDED( mNuiSensor->NuiInitialize( flag ) ) ){

				hr = mNuiSensor->NuiImageStreamOpen(
					NUI_IMAGE_TYPE_COLOR, NUI_IMAGE_RESOLUTION_640x480,
					0, 2, 0, &mColor );

				hr = mNuiSensor->NuiImageStreamOpen(
					NUI_IMAGE_TYPE_DEPTH, NUI_IMAGE_RESOLUTION_640x480,
					0, 2, 0, &mDepth );

				//�C�x���g�쐬����
				mEvent = CreateEvent( NULL, TRUE, FALSE, NULL );
				hr = mNuiSensor->NuiSetFrameEndEvent( mEvent, 0 );
	
				//�X�P���g��������
				hr = mNuiSensor->NuiSkeletonTrackingEnable( mEvent, NULL );
			}		
		}

		ZeroMemory(temp, sizeof(temp));
		if( NULL == mNuiSensor || FAILED( hr ) ){
			//�ǂ����ŃG���[�ł���
			hr = E_FAIL;
			STRONG_ASSERT( hr != E_FAIL && "Kinect�̃X�g���[���̏������Ɏ��s���܂���" );
		}
		//�X�P���g���̏�����
		for( int i = 0; i < SKELETON_INDEX_NONE; ++i ){
			mScreenPoints[ i ].x = mScreenPoints[ i ].y = 0.f;
		}
		mDepthPixels = NEW BYTE[ w * h * 4]; 

		mCameraBitmap = Bitmap::create( 640, 480, 0 );
		mDepthBitmap = Bitmap::create( 640, 480, 0 );
	}
	void createInstance(){
		HRESULT hr;
		//�L�l�N�g������
		INuiSensor* nui = NULL;

		//�ڑ�����Ă���Kinect�̑䐔���J�E���g
		int sensorCount = 0;
		hr = NuiGetSensorCount( &sensorCount );
		STRONG_ASSERT( hr != E_POINTER && "Kinect���ڑ�����Ă��܂���" );
		//�ڑ�����Ă���䐔�ɉ����ď�����
		for( int i = 0; i < sensorCount; ++i ){
			hr = NuiCreateSensorByIndex( i, &nui );
			STRONG_ASSERT( hr != E_POINTER && "�p�����[�^�n���h�����Ȃ���" );
			STRONG_ASSERT( hr != E_OUTOFMEMORY && "����������" );
			//��������[
			hr = nui->NuiStatus();
			if( hr == S_OK ){
				mNuiSensor = nui;
				break;
			}
			mNuiSensor->Release();
		}
	}
	~KinectManagerImpl(){
		if( 0 != mDepthPixels ){
			SAFE_DELETE( mDepthPixels );
		}
		if( mNuiSensor ){
			mNuiSensor->NuiShutdown();
		}
		mNuiSensor = 0;
	}
	//�X�V
	void update(){
		if ( WAIT_OBJECT_0 == WaitForSingleObject(mEvent, 0 ) ){
		 	processColor();
			processDepth();
			processSkeleton();
		}
	}
	//�F�����X�V����
	void processColor(){
		HRESULT hr;
		NUI_IMAGE_FRAME imageFrame;

		//�J��������F�����擾
		hr = mNuiSensor->NuiImageStreamGetNextFrame(
			mColor, 0, &imageFrame );
		if( FAILED( hr ) ){
			return;
		}

		INuiFrameTexture* texture = imageFrame.pFrameTexture;

		//�������b�N
		NUI_LOCKED_RECT lockedRect;
		texture->LockRect( 0, &lockedRect, NULL, 0 );

		if( lockedRect.Pitch != 0 ){
			mCameraBitmap.copyFromMemory( static_cast< BYTE* >( lockedRect.pBits ) );
		}
		//�������b�N����
		texture->UnlockRect( 0 );

		hr = mNuiSensor->NuiImageStreamReleaseFrame( mColor, &imageFrame );
	}
	void processDepth(){
		HRESULT hr;
		NUI_IMAGE_FRAME depthFrame;

		//�J�����[�x�����擾
		hr = mNuiSensor->NuiImageStreamGetNextFrame( mDepth, 0, &depthFrame);
		if( FAILED( hr ) ){
			return;
		}

		BOOL nearMode;
		INuiFrameTexture* texture;

		//�[�x�����e�N�X�`���Ɋi�[����
		hr = mNuiSensor->NuiImageFrameGetDepthImagePixelFrameTexture(
			mDepth, &depthFrame, &nearMode, &texture );

		//�[�x�����擾
		NUI_LOCKED_RECT lockedRect;
		texture->LockRect( 0, &lockedRect, 0, 0 );
		if( lockedRect.Pitch != 0 ){
			//Near���[�h�̏ꍇ�A�ő�[�x�ƍŏ��[�x��ύX
			int minDepth = (nearMode ? NUI_IMAGE_DEPTH_MINIMUM_NEAR_MODE : NUI_IMAGE_DEPTH_MINIMUM) >> NUI_IMAGE_PLAYER_INDEX_SHIFT;
			int maxDepth = (nearMode ? NUI_IMAGE_DEPTH_MAXIMUM_NEAR_MODE : NUI_IMAGE_DEPTH_MAXIMUM) >> NUI_IMAGE_PLAYER_INDEX_SHIFT;

			//	�s�N�Z������̊J�n�n�_�ƏI���n�_���쐬
			unsigned char* rgbrun = mDepthPixels;
			const NUI_DEPTH_IMAGE_PIXEL * pBufferRun = reinterpret_cast<const NUI_DEPTH_IMAGE_PIXEL *>(lockedRect.pBits);
			const NUI_DEPTH_IMAGE_PIXEL * pBufferEnd = pBufferRun + (640 * 480);

			int i=0;
			while ( pBufferRun < pBufferEnd )
			{
				USHORT depth = pBufferRun->depth;
				mDepthBuffer[i] = depth;
				BYTE intensity = static_cast<BYTE>(depth >= minDepth && depth <= maxDepth ? depth % 256 : 0);

				// Write out blue byte
				*(rgbrun++) = intensity;

				// Write out green byte
				*(rgbrun++) = intensity;

				// Write out red byte
				*(rgbrun++) = intensity;
				
				++rgbrun;

				++pBufferRun;
				i++;
			}
			mDepthBitmap.copyFromMemory( mDepthPixels );
		}


		//	��`���̃��b�N������
		texture->UnlockRect(0);
		texture->Release();

		mNuiSensor->NuiImageStreamReleaseFrame(mDepth, &depthFrame);
	}
	//UserID���܂��ŏ��̏�ԂŖ��ݒ�Ȃ炻���Őݒ肵�Ă�����
	void processSkeleton(){
		HRESULT hr;

		NUI_SKELETON_FRAME skeletonFrame = { 0 };
		if( NULL == mNuiSensor ){
			return;
		}

		//�X�P���g�������擾����
		hr = mNuiSensor->NuiSkeletonGetNextFrame( 0, &skeletonFrame );
		if( FAILED( hr ) ){
			return;
		}
		hr = mNuiSensor->NuiTransformSmooth( &skeletonFrame, NULL );
		
		NUI_SKELETON_DATA& skeletonData = skeletonFrame.SkeletonData[0];
		int number = -1;
		bool found = false;
		if( !mTrackingFlag ){
			for (int i = 0 ; i < NUI_SKELETON_COUNT; ++i){
				//	���i�ǐՂ̏��
				NUI_SKELETON_DATA& skeletonData = skeletonFrame.SkeletonData[i];
				NUI_SKELETON_TRACKING_STATE trackingState = skeletonFrame.SkeletonData[i].eTrackingState;
				skeletonData = skeletonFrame.SkeletonData[ i ];
				//�g���b�L���O�`�F�b�N
				if ( trackingState == NUI_SKELETON_TRACKED ){
					mTrackingID = skeletonData.dwTrackingID;
					mUserIndex = skeletonData.dwUserIndex;
					mTrackingFlag = true;
					return;
				}
			}
		} else {
			bool notfound = true;
			for (int i = 0 ; i < NUI_SKELETON_COUNT; ++i){
				//	���i�ǐՂ̏��
				NUI_SKELETON_DATA& skeletonData = skeletonFrame.SkeletonData[i];
				NUI_SKELETON_TRACKING_STATE trackingState = skeletonFrame.SkeletonData[i].eTrackingState;
				skeletonData = skeletonFrame.SkeletonData[ i ];
				if( ( skeletonData.dwTrackingID == mTrackingID ) && ( skeletonData.dwUserIndex = mUserIndex ) ){
					notfound = false;
					if( trackingState == NUI_SKELETON_TRACKED ){
					//	�X�P���g���̍��i���W���X�N���[�����W�n�ɕϊ�
						for (int j = 0; j < NUI_SKELETON_POSITION_COUNT; ++j){	
							if( skeletonData.eSkeletonPositionTrackingState[j] != NUI_SKELETON_POSITION_NOT_TRACKED ){
								skeletonToScreen( skeletonData.SkeletonPositions[j], j );
							}
						}
					} else if( trackingState == NUI_SKELETON_POSITION_COUNT ){
						//	�X�P���g���̍��i���W���X�N���[�����W�n�ɕϊ�
						for (int j = 0; j < NUI_SKELETON_POSITION_COUNT; ++j){	
							if( skeletonData.eSkeletonPositionTrackingState[j] != NUI_SKELETON_POSITION_NOT_TRACKED )
							{
								skeletonToScreen(skeletonData.SkeletonPositions[j], j);
							}
						}
					} 
				}
			}
			if( notfound ){
				initTrackingFlag();
			}
		}
	}
	bool tracking()  {
		return mTrackingFlag;
	}
	Math::Vector2 index( SkeletonIndex index ) const {
		return mScreenPoints[ index ];
	}
	void skeletonToScreen( _Vector4& skeletonPoint, int i ){
		float x, y;
		LONG screenPointX, screenPointY;

		//���i���W���X�N���[�����W�ւƕϊ�����
		NuiTransformSkeletonToDepthImage( skeletonPoint, &x, &y, NUI_IMAGE_RESOLUTION_640x480 );

		mNuiSensor->NuiImageGetColorPixelCoordinatesFromDepthPixelAtResolution(
			NUI_IMAGE_RESOLUTION_640x480, NUI_IMAGE_RESOLUTION_640x480, 0, (LONG)x, (LONG)y, 0, &screenPointX, &screenPointY );

		//�]���̈�ɍ��킹�čœK��
		x = ( mWidth/640.f ) * static_cast< float >( screenPointX );
		y = ( mHeight/480.f ) * static_cast< float >( screenPointY );

		mScreenPoints[ i ].x = x; mScreenPoints[ i ].y = y;
	}
	void setWidth( int w ){
		mWidth = w;
	}
	void setHeight( int h ){
		mHeight = h;
	}
	Bitmap colorTexture() {
		return mCameraBitmap;
	}
	Bitmap depthTexture() {
		return mDepthBitmap;
	}
	Math::Vector2 skeletonPos( int i ){
		return mScreenPoints[ i ];
	}
	//�X�P���g���̐[�x�l���擾
	int depthSkeleton( int i ){
		if( i < 0 ){
			return 0;
		}

		if( mScreenPoints[ i ].y >= 780 || mScreenPoints[ i ].x >= 1280)
		{
			return temp[i] * 6;
		}

		//�Ƃ���X�P���g���̂Ƃ���[�x�l���������܂���
		int x = static_cast<int>(mScreenPoints[ i ].x / ( mWidth/640.f ));
		int y = static_cast<int>(mScreenPoints[ i ].y / ( mHeight/480.f ));
		//�z��Y�����G���[���
		if( x < 0 || y < 0 || x >= 480 || y >= 640  ){
			return 0;
		}

		
		if(mDepthBuffer[(y * 640) + x] >>8 <= 0)
		{
			return temp[i] * 6;
		}
		temp[i] = (mDepthBuffer[(y * 640) + x] >>8);
		return (mDepthBuffer[(y * 640) + x] >>8)*6;
	}
	//�J�����̊p�x�ύX
	void setCamera( float radian ){
		//mNuiSensor->NuiCameraElevationSetAngle( NUI_
	}
	//�g���b�L���O�t���O
	void initTrackingFlag(){
		mUserIndex = -1;
		mTrackingID = -1;
		mTrackingFlag = false;
		processSkeleton();
	}
	//�L�l�N�g�Z���T�[�p�̃C���^�[�t�F�[�X
	INuiSensor* mNuiSensor;
	HANDLE mColor;
	HANDLE mDepth;
	HANDLE mSkeleton;
	HANDLE mNextColorFrameEvent;
	HANDLE mEvent;
	//���[�U�[�ԍ�
	bool mTrackingFlag;
	unsigned long mUserIndex;
	unsigned long mTrackingID;
	//�X�P���g��[�X�N���[�����W�n�ϊ���]
	Math::Vector2 mScreenPoints[ NUI_SKELETON_POSITION_COUNT ];
	//�f�v�X�o�b�t�@
	USHORT mDepthBuffer[640*480];
	BYTE*			mDepthPixels;
	int temp[NUI_SKELETON_POSITION_COUNT];
	int mWidth;
	int mHeight;
	//�r�b�g�}�b�v��Kinect�̉摜
	Bitmap mDepthBitmap;
	Bitmap mCameraBitmap;

};
extern KinectManagerImpl* gManagerImpl; //�C���X�^���X

}
}


#endif
