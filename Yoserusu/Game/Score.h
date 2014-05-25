#ifndef SCORE_H_
#define SCORE_H_

#include "Mashiro/Mashiro.h"

class Score{
public:
	static Score* instance();
	static void create();
	static void destory();
	//スコアを加算
	void Add();
	//スコアをゲット
	int getScore()const;
	void Init();
private:
	Score();
	~Score();
	//スコア
	int mCount;
	static Score* mInstance;
};
#endif
