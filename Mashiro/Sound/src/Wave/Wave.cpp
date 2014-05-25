#include "Mashiro/Sound/src/Wave/Wave.h"
#include "Mashiro/Mashiro.h"

namespace Mashiro {
namespace Sound{

Wave::Wave() :
mFp( 0 ),
mSoundSize( 0 ),
mHeaderSize( 0 ),
mBlockSize( 0 ),
mLength( 0 ){
}

Wave::~Wave(){
	if( 0 != mFp ){
		rlease();
	}
}

bool Wave::readFormat( WAVEFORMATEX& format ){
	//形式チェック
	char checkTemp[ 4 ];

	fread( checkTemp, sizeof( char ) * 4, 1, mFp );

	if( memcmp( checkTemp, "RIFF", 4 ) ){
		return false;
	}

	fseek( mFp, 4, SEEK_CUR );
	fread( checkTemp, sizeof( char ) * 4, 1, mFp );

	if( memcmp( checkTemp, "WAVE", 4 ) ){
		return false;
	}

	//フォーマットを読み取る
	int formatSize = 0;

	fseek( mFp, 4, SEEK_CUR );

	fread( &formatSize, sizeof( int ), 1, mFp );
	fread( &format, sizeof( WAVEFORMATEX ), 1, mFp );

	if( WAVE_FORMAT_PCM != format.wFormatTag ){
		return false;
	}

	format.cbSize = 0;

	fseek( mFp, formatSize - sizeof( WAVEFORMATEX ), SEEK_CUR );


	// サウンドのサイズを調べる	
	char	chunkName[4];
	int		chunkSize;

	while( true ){
		if( feof( mFp ) )
			return false;

		fread( chunkName, sizeof( char ) * 4, 1, mFp );
		fread( &chunkSize, sizeof( int ), 1, mFp );

		if( memcmp( chunkName, "data", 4 ) == 0 )
			break;

		fseek( mFp, chunkSize, SEEK_CUR );
	}

	mSoundSize	= chunkSize;
	mHeaderSize	= ftell( mFp );

	mBlockSize	= format.nBlockAlign;
	mLength		= mSoundSize / format.nBlockAlign;

	return true;
}

bool Wave::open( const char* path, WAVEFORMATEX& format ){
	fopen_s( &mFp, path, "rb" );
	
	return readFormat( format );
}

void Wave::close(){
	if( mFp == 0 ){
		return;
	}

	fclose( mFp );
	mFp = 0;

	return;
}

unsigned int Wave::decode( unsigned char* buffer, unsigned int size ){
	return fread( buffer, sizeof( unsigned char ), size, mFp );
}

unsigned int Wave::getLength(){
	return mLength;
}

void Wave::rlease(){
	if( 0 == mFp ){
		return;
	}

	fclose( mFp );
	mFp = 0;
}

void Wave::seek( int offset ){
	offset *= mBlockSize;

	fseek( mFp, mHeaderSize + offset, SEEK_SET );

	return;
}

bool Wave::checkStreamEnd(){
	const long temp = mSoundSize + mHeaderSize;

	if( ftell( mFp ) < temp ){
		return false;
	}

	return true;
}

}
}