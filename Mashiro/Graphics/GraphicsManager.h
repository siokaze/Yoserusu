#ifndef INCLUDE_MASHIRO_GRAPHICS_MANAGER_INCLUDE_H_
#define INCLUDE_MASHIRO_GRAPHICS_MANAGER_INCLUDE_H_

#include "Mashiro/Graphics/Enum.h"

namespace Mashiro {
	namespace Math {
		class Matrix;
		class Vector4;
		class Vector3;
		class Vector2;
	}
}

namespace Mashiro{
namespace Graphics{

class Texture;
class Bitmap;
class VertexBuffer;
class IndexBuffer;
class Shader;
class RenderTarget;

class Manager{
public:
	Manager();
	static Manager instance();

	void end();
	//�ݒ肵���e�N�X�`���A���_�o�b�t�@�A�C���f�b�N�X�o�b�t�@�ŕ`��
	void drawIndexed( int indexCount, int start, PrimitiveType = PRIMITIVE_TRIANGLE );
	//�ݒ肵���e�N�X�`���A���_�o�b�t�@�ŕ`��
	void draw( PrimitiveType = PRIMITIVE_TRIANGLE );

	//�����_�����O�^�[�Q�b�g�e�N�X�`���[�Z�b�g
	void setTexture( RenderTarget targetTexture, int stage );
	//�e�N�X�`���[�Z�b�g
	void setTexture( Texture, int stage );
	//���_�o�b�t�@�Z�b�g
	void setVertexBuffer( VertexBuffer );
	//�C���f�b�N�X�o�b�t�@�Z�b�g
	void setIndexBuffer( IndexBuffer );
	//�V�F�[�_�Z�b�g
	void setShader( Shader shader );
	//�����_�����O�^�[�Q�b�g [����] ����, depth�^�[�Q�b�g���g�����ǂ����H
	void setRenderTarget( int num, RenderTarget target[] );
	//�o�b�N�o�b�t�@�[�̃����_�����O�^�[�Q�b�g�ɖ߂�
	void setBackBufferTarget();

	//World�}�g���b�N�X�Z�b�g
	void setWorldMatrix( const Math::Matrix& );
	//Projection�}�g���b�N�X�Z�b�g
	void setProjectionMatrix( const Math::Matrix& );
	//View�}�g���b�N�X�Z�b�g
	void setViewMatrix( const Math::Matrix& );
	//���C�g�ʒu�Z�b�g
	void setLight( const Math::Vector4& );
	//�����Z�b�g
	void setEyePos( const Math::Vector4& );
	//Diffuse�J���[�Z�b�g
	void setDiffuse( const Math::Vector4& );
	//���ߗ�
	void setTransparency( float );
	//Ambient�J���[�Z�b�g
	void setAmbient( const Math::Vector4& );
	//�X�y�L�����[
	void setSpecular( const Math::Vector4& );

	//�r���[�|�[�g�Z�b�g
	void setViewPort( int x, int y, int w, int h );
	//�r���[�|�[�g�擾
	void getViewPort( int* x, int* y, int* w, int* h );
	//�E�B���h�E��
	void enableWindowScreen( bool );
	//�u�����h���[�h
	void setBlendMode( BlendMode );
	//�K�����O���[�h
	void setCullMode( CullMode );

	//viewMatrix���o��
	Mashiro::Math::Matrix viewMatrix() const;

	void enableAlphaBlend( bool );
	void enableDepthWriteMask( bool );
	void enableDepthTest( bool );
	void enableDepthWrite( bool );
	
	int width() const;
	int height() const;
	
	static void create( void* hwnd, int w, int h, bool window );
	static void destroy();
	void getPointerModifier( float* scale, Mashiro::Math::Vector2* offset ) const;
};

}
}

#endif