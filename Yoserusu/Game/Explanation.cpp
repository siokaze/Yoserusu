#include "Explanation.h"
#include "Mashiro/Math/Vector2.h"
#include "Mashiro/Graphics/SpriteManager.h"
#include "Game\Score.h"

Explanation::Explanation():color(0),isCatch(false),count(0){
	mTextures[REDTEX] = Mashiro::Graphics::Bitmap::create("res/image/red.png");
	mTextures[BLUETEX] = Mashiro::Graphics::Bitmap::create("res/image/blue.png");
	mTextures[GREENTEX] = Mashiro::Graphics::Bitmap::create("res/image/green.png");
	mTextures[PULLTEX] = Mashiro::Graphics::Bitmap::create("res/image/hira.png");
	mTextures[CATCHTEX] = Mashiro::Graphics::Bitmap::create("res/image/catch.png");
}

Explanation::~Explanation(){

}

void Explanation::Init(){
	color = 0;
	isCatch = false;
}

void Explanation::ChangeTexture(int _color,bool _isCatch){
	color = _color;
	isCatch = _isCatch;

}

void Explanation::Draw(){

	if(Score::instance()->getScore() >= 3) return;

	Mashiro::Graphics::Sprite colorsp = Mashiro::Graphics::Sprite::instance();
	Mashiro::Graphics::Sprite catchsp = Mashiro::Graphics::Sprite::instance();

	colorsp.setTrance(1.f);
	
	colorsp.setBitmap(mTextures[color]);
	colorsp.setBitmapRectangle(Vector2(440,280));
	colorsp.draw();

	if(isCatch){
		catchsp.setBitmap(mTextures[PULLTEX]);
	}else{
		catchsp.setBitmap(mTextures[CATCHTEX]);
	}
	catchsp.setBitmapRectangle(Vector2(440,500));
	catchsp.draw();
}