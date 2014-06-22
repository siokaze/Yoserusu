#ifndef ENUM_H_
#define ENUM_H_

enum Mode{
	Keep,
	Release,
	Normal,
	Catch,
	
};

enum EXPLANATION_TEXTURES{
	REDTEX = 0,
	BLUETEX = 1,
	GREENTEX = 2,
	PULLTEX = 3,
	CATCHTEX = 4, 

	MAX = 5
};

enum WallColor
{
	RED = 0,
	BLUE = 1,
	GREEN = 2
};

//åªç›ÇÃîFèÿÇÃèÛë‘
enum AutheMode {
	MODE_WAIT,
	MODE_NOW,
	MODE_END,
};

enum WallState{
	WALL_OPEN,
	WALL_FALSE,
	WALL_NONE,
};

#endif