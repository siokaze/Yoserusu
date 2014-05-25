#ifndef INCLUDE_MASHIRO_MASHIRO_H_
#define INCLUDE_MASHIRO_MASHIRO_H_

namespace Mashiro {
	enum Exception{
		EXCEPTION_IGNORE,
		EXCEPTION_EXIT,
	};
	void halt( const char* filename, int line, const char* message );
	void messeage( const char* fileName, int line, const char* message );
}

//マクロ類
#define STRONG_ASSERT( exp ) ( ( !! ( exp ) ) || ( Mashiro::halt( __FILE__, __LINE__, #exp ), 0 ) ) //この行の意味を理解するのは相当難しい
#define MESSAGE( exp ) { Mashiro::messeage( __FILE__, __LINE__, exp ); }
#define HALT( exp ) { Mashiro::halt( __FILE__, __LINE__, #exp ); }
#define SAFE_DELETE( x ) { delete ( x ); ( x ) = 0; }
#define SAFE_DELETE_ARRAY( x ) { delete[] ( x ); ( x ) = 0; }
#define OPERATOR_DELETE( x ) { operator delete( x ); ( x ) = 0; }

//デバグとリリースで分岐するもの
#ifndef NDEBUG
#define NEW new( __FILE__, __LINE__ )
#define OPERATOR_NEW( x ) operator new( ( x ), __FILE__, __LINE__ ) 
#define ASSERT( exp ) ( ( !! ( exp ) ) || ( Mashiro::halt( __FILE__, __LINE__, #exp ), 0 ) )
#else //NDEBUG
#define NEW new
#define OPERATOR_NEW( x ) operator new( x ) 
#define ASSERT( exp ) 
#endif //NDEBUG

#include "Base/Array.h"
#include "Base/Array2d.h"
#include "Base/AutoPtr.h"
#include "Base/SharedPtr.h"
#include "Base/MemoryManager.h"
#include <new>

#endif