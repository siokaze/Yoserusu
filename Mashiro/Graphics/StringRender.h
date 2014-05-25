#ifndef INCLUDE_MASHIRO_GRAPHICS_STRING_RENDER_H_
#define INCLUDE_MASHIRO_GRAPHICS_STRING_RENDER_H_

#include <sstream>
#include "Mashiro/Graphics/Enum.h"

namespace Mashiro{
	namespace Math{
		class Vector2;
	}
}

namespace Mashiro{
namespace Graphics {
	
class StringRender{
public:
	static StringRender create( Font font, int size );

	int precision( int );
	StringRender& operator<<( char );
	StringRender& operator<<( unsigned char );
	StringRender& operator<<( int );
	StringRender& operator<<( unsigned );
	StringRender& operator<<( short );
	StringRender& operator<<( unsigned short );
	StringRender& operator<<( float );
	StringRender& operator<<( double );
	StringRender& operator<<( const std::string& );
	StringRender& operator<<( const char* );
	StringRender& operator<<( char* );
	StringRender& operator<<( StringRender& ( *f )( StringRender& ) );
	StringRender& endl();
	//色を変更する。アルファが0だと勝手に255にするので、0xff4477のように渡せて楽
	void setColor( float r = 1.f, float g = 1.f, float b = 1.f, float a = 1.f );
	//Buffer内の貯めこんだものを描画
	void draw( int col = 0, int row = 0 );

	//以下ユーザは気にするな
	StringRender();
	~StringRender();
	StringRender( const StringRender& );
	operator void*() const;
	StringRender& operator=( const StringRender& );
	void release();
	bool operator==( const StringRender& ) const;
	bool operator!=( const StringRender& ) const;
	class Impl;
	StringRender( const Impl* );
private:
	friend class Manager;
	Impl* mImpl;
};

StringRender& endl( StringRender& );

}
}

#endif
