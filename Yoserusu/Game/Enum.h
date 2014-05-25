#ifndef ENUM_H_
#define ENUM_H_

enum Mode{
	Keep,
	Release,
	Normal,
	Catch,
	
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