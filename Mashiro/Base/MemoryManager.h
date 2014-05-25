#ifndef INCLUDED_MASHIRO_BASE_MEMORYMANAGER_H
#define INCLUDED_MASHIRO_BASE_MEMORYMANAGER_H

namespace Mashiro{

//newをつかさどるクラス
class MemoryManager{
public:
	static MemoryManager instance();

	///情報の書き出し(ファイル名を与えればファイルへ、与えなければデバグ出力へ)
	void write( const char* filename = 0 );
	///全確保容量(内部予約容量を含む)
	int totalSize() const;
};

}

void* operator new( size_t size, const char* filename, int line );
void* operator new[]( size_t size, const char* filename, int line );
void operator delete( void*, const char*, int );
void operator delete[]( void*, const char*, int );
void* operator new( size_t size );
void* operator new[]( size_t size );
void operator delete( void* );
void operator delete[]( void* );

#endif
