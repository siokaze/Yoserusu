#include "Ogg.h"
#include "vorbis/vorbisfile.h"

namespace Mashiro {
namespace Sound {

Ogg::Ogg() : mOpen( false ){
}

Ogg::~Ogg(){
	rlease();

	return;
}

bool Ogg::readFormat( FILE* fp, WAVEFORMATEX& format ){
	if( ov_open( fp, &mFp, 0, 0 ) ){
		fclose( fp );

		return false;
	}

	mOpen = true;

	const vorbis_info*	info = ov_info( &mFp, -1 );

	format.wFormatTag		= WAVE_FORMAT_PCM;
	format.nChannels		= static_cast< WORD >( info->channels );
	format.nSamplesPerSec	= info->rate;
	format.nAvgBytesPerSec	= info->channels * info->rate * 2;
	format.nBlockAlign		= static_cast< WORD >( info->channels * 2 );
	format.wBitsPerSample	= 16;
	format.cbSize			= 0;

	return true;
}

bool Ogg::open( const char* fileName, WAVEFORMATEX& format ){
	FILE* fp = 0;

	fopen_s( &fp, fileName, "rb" );

	return readFormat( fp, format );
}

void Ogg::rlease() {
	if( !mOpen ){
		return;
	}

	ov_clear( &mFp );

	mOpen = false;

	return;
}

unsigned int Ogg::decode( unsigned char* buffer, unsigned int size ){
	unsigned int readSize = 0;

	size -= 4096;

	for( int i = 0; i < 8; ++i ){
		readSize += ov_read(
			&mFp,
			reinterpret_cast< char* >( buffer+ readSize ),
			size,
			0,
			sizeof( unsigned short ),
			1,
			0 );
		if( readSize > size ){
			break;
		}
	}
	return readSize;
}

void Ogg::seek( int offset ){
	ov_pcm_seek( &mFp, offset );

	return;
}

bool Ogg::checkStreamEnd(){
	if( 4096 > ( ov_pcm_total( &mFp, 0 ) - ov_pcm_tell( &mFp ) ) ){
		return true;
	}
	return false;
}

unsigned int Ogg::getLength(){
	return static_cast< unsigned int >( ov_pcm_total( &mFp, -1 ) );
}

}
}