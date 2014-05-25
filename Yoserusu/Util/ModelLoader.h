#ifndef INCLUDE_MODEL_LOADER_H_
#define INCLUDE_MODEL_LOADER_H_

#include "Mashiro/Scene/PMDFileLoader.h"
#include "Mashiro/Scene/Model.h"

class ModelLoader {
public:

	static ModelLoader* instance();
	static void create();
	static void destory();
	//ファイルをロード
	Mashiro::Scene::Model createModel( const char* fileName );
private:
	ModelLoader();
	~ModelLoader();
	static ModelLoader* mInstance;

	Mashiro::Scene::PMDFileLoader mLoader;
};

#endif