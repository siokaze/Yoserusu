#ifndef INCLUDE_MASHIRO_WORKSPACE_H_
#define INCLUDE_MASHIRO_WORKSPACE_H_

#include <string>

namespace Mashiro{

class WorkSpace{
public:
	class Configuration{
	public:
		///幅設定
		void setWidth( int );
		///高さ設定
		void setHeight( int );
		///ウィンドウタイトル設定
		void setTitle( const char* title );
		///垂直同期有効化設定
		void enableVSync( bool = true );
		///フルスクリーンアンチエイリアス設定
		void enableAntiAlias( bool = true );
		///起動時のスクリーン設定
		void enableWindowScreen( bool = true );
		///HALT,ASSERTで即死せずに例外を投げる機能の有効化。売り物に即死コードは入れられない、という人のためのもの。
		void enableThrowWhenHalt( bool = true );
		///追加スレッド数設定
		void setExtraThreadNumber( int );
		///コマンドラインオプションを取得
		const char* commandLineString() const;
	};
	WorkSpace();
	//インスタンス取得
	static WorkSpace instance();

	//ユーザー定義関数。作らないとエラーでるからな
	void configure( Configuration* );
	//ユーザー定義
	void update();
	//Kinect生成
	void kinectCreate();

	//ウィンドウフラグ
	void enableWindowFlag( bool );
	//画面幅取得
	int width() const;
	//画面高さ取得
	int height() const;
	//タイトル
	const char* title() const;
	//ウィンドウですか？
	bool isWindowScreen() const;
	//終了命令
	void requestEnd();
	//終了命令をチェック
	bool isEndRequested() const;
	//フレームレート取得
	int frameRate() const;
	//フレームレートの設定
	void setFrameRate( int );
	//時間取得
	unsigned time() const;
	//スリープ
	void sleep( int i );

	void start( void* handle );
	void preUpdate();
	void postUpdate();
	static void create();
	static void destroy();
};

}

#endif