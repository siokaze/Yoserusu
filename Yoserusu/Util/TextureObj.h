#ifndef INCLUDE_UTIL_TEXTURE_OBJ_H_
#define INCLUDE_UTIL_TEXTURE_OBJ_H_

#include "Mashiro/Graphics/Texture.h"

/*
* @class TextureObj
* @brif �e�N�X�`���J�v�Z����
*/
class TextureObj {
public:
	TextureObj(){}
	TextureObj( const char* fileName ){
		create( fileName );
	}
	void create( const char* fileName ){ 
		mTexture = Mashiro::Graphics::Texture::create( fileName ); 
	}
private:
	friend class ModelObj;
	Mashiro::Graphics::Texture mTexture;
};

#endif