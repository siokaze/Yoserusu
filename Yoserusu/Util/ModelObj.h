#ifndef INCLUDE_MODEL_OBJ_H_
#define INCLUDE_MODEL_OBJ_H_

#include "Transform.h"
#include "TextureObj.h"
#include "Mashiro/Scene/Model.h"
#include "Shader/include/CocTrans.h"
#include "Mashiro/Base/SharedPtr.h"

/*
* @class ModelObj
* @brif ���f���I�u�W�F�N�g
*/
class ModelObj : public Transform {
public:
	ModelObj();
	ModelObj( const char* fileName );
	/*
	 * @brif ���f���쐬
	 * @param const char* fileName - �t�@�C����
	 */
	void create( const char* fileName );
	/*
	* @brif �J���[�l�Z�b�g
	* @param const Vector3& color - �J���[
	*/
	void setColor( const Mashiro::Math::Vector3& color );
	/*
	 * @brif ���ߒl�Z�b�g
	 * @param float t 
	 */
	void setTrance( float t );
	/*
	* @brif �e�N�X�`���Z�b�g
	*/
	void setTexture( const TextureObj& tex, int stage = 0 );
	/*
	 * @brif �`��
	 */
	void draw( CocTrans::Type );

private:
	Mashiro::Scene::Model mModel;
	//�V�F�[�_
	Mashiro::SharedPtr< CocTrans > mCocTrans;
};

#endif