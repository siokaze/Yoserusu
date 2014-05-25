#ifndef INCLUDE_MASHIRO_GRAPHICS_TEXTURE_H_
#define INCLUDE_MASHIRO_GRAPHICS_TEXTURE_H_

namespace Mashiro{

namespace Graphics {

class Texture {
public:
	//名前から生成
	static Texture create( const char* fileName );
	//空のテクスチャー生成！
	static Texture create( int w, int h );
	//名前ゲット
	const char* name() const;

	//情報取得
	int width() const;
	int height() const;
	//自前でテクスチャーを作ります
	unsigned char* lockChar();
	///書き込み関数
	void lock( unsigned** address, int* pitch, int mipLevel = 0 );
	///部分書き込み関数
	void lock( 
		unsigned** address, 
		int* pitch, 
		int x,
		int y,
		int width,
		int height,
		int mipLevel = 0 );
	void unlock();

	//以下ユーザは意識しない関数群
	Texture();
	Texture( const Texture& );
	~Texture();
	operator void*() const;
	Texture& operator=( const Texture& );
	void release();
	bool operator==( const Texture& ) const;
	bool operator!=( const Texture& ) const;
	bool operator<( const Texture& ) const;

	class Impl;
	Texture( const Impl* );
private:
	friend class Manager;
	Impl* mImpl;
};

}
}


#endif
