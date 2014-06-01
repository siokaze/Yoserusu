#ifndef INCLUDE_MASHIRO_GRAPHICS_BITMAP_H_
#define INCLUDE_MASHIRO_GRAPHICS_BITMAP_H_

namespace Mashiro{
namespace Graphics {

class Bitmap {
public:
	//�r�b�g�}�b�v���쐬
	static Bitmap create( const char* fileName );
	//�r�b�g�}�b�v�C���[�W���쐬
	static Bitmap create( int width, int height, long stride );

	//unsigned char*����R�s�[
	void copyFromMemory( unsigned char* byte );

	//�T�C�Y
	int height() const;
	int width() const;

	//�ȉ����[�U�͈ӎ����Ȃ��֐��Q
	Bitmap();
	Bitmap( const Bitmap& );
	~Bitmap();
	operator void*() const;
	Bitmap& operator=( const Bitmap& );
	void release();
	bool operator==( const Bitmap& ) const;
	bool operator!=( const Bitmap& ) const;
	bool operator<( const Bitmap& ) const;

	class Impl;
	Bitmap( const Impl* );
private:
	friend class Sprite;
	friend class Kinect;
	Impl* mImpl;
};

}
}

#endif