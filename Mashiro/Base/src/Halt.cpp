#include <windows.h>
#include <stdio.h>
#include "Mashiro/Mashiro.h"

bool gThrowWhenHalt = false;

namespace Mashiro {

void halt( const char* fileName, int line, const char* message ){
	char buf[ 8192 ];
	sprintf_s( buf, 8192, "%s:%d : %s", fileName, line, message );
	if( gThrowWhenHalt ){
		::OutputDebugStringA( buf );
		::OutputDebugStringA( "\n" );
		throw EXCEPTION_IGNORE; //����
	} else {
		//���b�Z�[�W�{�b�N�X
		MessageBoxA( 0, buf, "Criticl Error!", MB_OK | MB_ICONERROR );
		//����
		*reinterpret_cast< int* >( 0 ) = 0;
	}
}

void messeage( const char* fileName, int line, const char* message ) {
	char buf[ 8192 ];
	sprintf_s( buf, 8192, "%s:%d : %s", fileName, line, message );
	//���b�Z�[�W�{�b�N�X
	MessageBoxA( 0, buf, "LuaScprictError", MB_OK | MB_ICONERROR );
}

}