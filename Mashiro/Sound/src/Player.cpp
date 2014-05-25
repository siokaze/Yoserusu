#include "Mashiro/Mashiro.h"
#include "Mashiro/Sound/Player.h"
#include "Mashiro/Sound/src/PlayerImpl.h"

namespace Mashiro {
namespace Sound{

Player Player::create( MusicSource source ){
	Player r;
	r.mImpl = NEW Impl( source.mImpl );
	return r;
}

void Player::start(){
	ASSERT( mImpl && " Sound::Player : This is empty Object" );
	return mImpl->start();
}

void Player::start( unsigned int loopPos, unsigned int length ){
	ASSERT( mImpl && " Sound::Player : This is empty Object" );
	return mImpl->start( loopPos, length );
}

void Player::pause(){
	ASSERT( mImpl && " Sound::Player : This is empty Object" );
	return mImpl->pause();
}

void Player::stop(){
	ASSERT( mImpl && " Sound::Player : This is empty Object" );
	return mImpl->stop();
}

void Player::setVolum( float volume ){
	ASSERT( mImpl && " Sound::Player : This is empty Object" );
	return mImpl->setVolum( volume );
}

void Player::setPitch( float s ){
	ASSERT( mImpl && " Sound::Player : This is empty Object" );
	return mImpl->setPitch( s );
}

#define TYPE Player
#include "Mashiro/Base/Impl/ReferenceTypeTemplate.h"

}
}