#ifndef INCLUDE_MASHIRO_GRAPHICS_VERTEX_H_
#define INCLUDE_MASHIRO_GRAPHICS_VERTEX_H_

#include "Mashiro/Math/Matrix.h"
#include "Mashiro/Math/Vector4.h"
#include "Mashiro/Math/Vector3.h"
#include "Mashiro/Math/Vector2.h"

namespace Mashiro{

namespace Graphics{

using namespace Mashiro::Math;

class SkinMeshVertex{
public:
	Mashiro::Math::Vector3 mPosition;		//	�|�W�V����
	Mashiro::Math::Vector3 mNormal;		//	�@��
	Mashiro::Math::Vector2 mUV;			//	�e�N�X�`�����W
	unsigned int mBoneIndex[4];		//	�{�[���̔ԍ�	
	Mashiro::Math::Vector4 mBoneWeight;	//	�{�[���̏d��
};

class Vertex {
public:
	Mashiro::Math::Vector4 mPosition; //�ʒu
	Mashiro::Math::Vector3 mNormal; //�@��
	Mashiro::Math::Vector4 mColor; //���_�F
	Mashiro::Math::Vector2 mUV; //UV
	Mashiro::Math::Vector3 mBinormal;
	Mashiro::Math::Vector3 mTangent;
};

struct BasicConstantBuffer{
	Matrix mWorld;
	Matrix mView;
	Matrix mProj;
	Vector4 mLight;
	Vector4 mDiffuse;
	Vector4 mAmbient;
	Vector4 mEye;
	Vector4 mSpecular;
};

struct FogConstant{
	Vector4 mFog;
	Vector4 mFogColor;
	Vector4 mHeight;
};

struct FbxSkinMeshConstant
{
	Matrix mBoneData[255];		//	�{�[���s��
};

struct DefferdPostConstant{
	Vector4 mLightPos[ 4 ];
};

struct shadowConstantBuffer {
	Matrix worldInv; // ���[���h�t�s�� 	
	Vector4 offset; // �V���h�E�}�b�v�A�N�Z�X�p�̃I�t�Z�b�g�ʒu 
	Matrix vlp; // �����ɑ΂��Ă̕ϊ��s��
};

struct circleShadowConstantBuffer{
	unsigned int gCount[ 4 ];
	Vector4 mModelToLight[ 50 ]; //�e�𗎂Ƃ����f���ƃ��C�g�����ԃx�N�g��
	Vector4 mModelPos[ 50 ]; //���f���̈ʒu
};

struct myConstantBuffer{

	//�Ƃ肠�����O������~�������̂�錾
	int mPart; //�`�悷����̂��Ƃɕ��򂵂�������
	int mDummy;
};

}
}

#endif
