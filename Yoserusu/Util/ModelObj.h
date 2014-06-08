#ifndef INCLUDE_MODEL_OBJ_H_
#define INCLUDE_MODEL_OBJ_H_

#include <memory>

#include "Transform.h"
#include "Mashiro/Graphics/Texture.h"
#include "Mashiro/Scene/Model.h"
#include "Shader/include/CocTrans.h"

/*
* @class ModelObj
* @brif モデルオブジェクト
*/
class ModelObj : public Transform {
public:
	ModelObj();
	ModelObj( const char* fileName );
	/*
	 * @brif モデル作成
	 * @param const char* fileName - ファイル名
	 */
	void create( const char* fileName );
	/*
	* @brif カラー値セット
	* @param const Vector3& color - カラー
	*/
	void setColor( const Mashiro::Math::Vector3& color );
	/*
	 * @brif 透過値セット
	 * @param float t 
	 */
	void setTrance( float t );
	/*
	* @brif テクスチャセット
	*/
	void setTexture( const Mashiro::Graphics::Texture& tex, int stage = 0 );
	/*
	 * @brif 描画
	 */
	void draw( CocTrans::Type );

private:
	Mashiro::Scene::Model mModel;
	//シェーダ
	std::shared_ptr< CocTrans > mCocTrans;
};

#endif