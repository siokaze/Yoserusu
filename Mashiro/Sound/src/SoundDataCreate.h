#include "Mashiro/Sound/src/Wave/Wave.h"
//#include "Mashiro/Sound/src/Ogg/Ogg.h"
#include "Mashiro/Sound/src/DecoderBase.h"

namespace Mashiro {
namespace Sound {

inline DecoderBase* createDecoder( const char* fileName ){
	char			buf[ 3 ];
	unsigned int	pathLen;
	FILE*			fp = 0;

	if( fopen_s( &fp, fileName, "rb" ) )
		return 0;

	pathLen = strlen( fileName );

	fread( buf, sizeof( char ) * 3, 1, fp );

	fclose( fp );

	if( 0 == memcmp( buf, "RIF", 3 ) ){
		return NEW Wave;	
	}
	//if( 0 == memcmp( buf, "Ogg", 3 ) ){
	//	return NEW Ogg;
	//}

	return 0;
}

}
}
