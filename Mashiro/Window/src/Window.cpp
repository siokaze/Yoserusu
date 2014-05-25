#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x400 //WM_MOUSEWHEELのために
#endif

#include <tchar.h>
#include <windows.h>
#include <mmsystem.h>
#include <string>
#include <crtdbg.h>
#undef min
#undef max

#include "Mashiro/Mashiro.h"
#include "Mashiro/Window/Resource.h"
#include "Mashiro/Window/Window.h"
#include "Mashiro/Graphics/GraphicsManager.h"
#include "Mashiro/Input/InputManager.h"

using namespace std;
using namespace Mashiro;

//WindowCreatorメンバ関数
namespace Mashiro{

namespace{ //無名

class Impl{
public:
	Impl() :
	mWidth( 800 ), 
	mHeight( 600 ), 
	mMinimized( false ),
	mActive( false ),
	mTitle( "ましろんアプリケーション" ),
	mCommandLine( "" ),
	mEndRequested( false ),
	mEnded( false ),
	mStarted( false ),
	mDragAndDropEnabled( false ),
	mMouseWheel( 0 ),
	mTimeBias( 0 ){
		//getTime()用バイアス値計算
		unsigned time = timeGetTime();
		mTimeBias = 0xffffffff - time;
		mTimeBias -= 60000; //60秒引く。

		//スレッドID取得
		mThreadId = GetCurrentThreadId();
	}
	~Impl(){
	}
	int mWidth;
	int mHeight;
	bool mMinimized;
	bool mActive;
	string mTitle;
	string mCommandLine;
	bool mEndRequested;
	bool mEnded;
	bool mStarted;
	bool mDragAndDropEnabled;
	int mMouseWheel;
	unsigned mTimeBias; //timeGetTimeにはかせるゲタ
	unsigned mThreadId;
	HWND mWindowHandle;
	RECT mWindowRect;
};

Impl* gImpl = 0; //唯一インスタンス

}

Window::Window(){
}

const char* Window::title() const {
	return gImpl->mTitle.c_str();
}

int Window::titleLength() const {
	return static_cast< int >( gImpl->mTitle.size() );
}

int Window::height() const {
	return gImpl->mHeight;
}

int Window::width() const {
	return gImpl->mWidth;
}

bool Window::isEndRequested() const {
	return gImpl->mEndRequested;
}

void Window::requestEnd(){
	gImpl->mEndRequested = true;
}

void Window::end(){
	gImpl->mEnded = true;
}

unsigned Window::time() const {
	return timeGetTime() + gImpl->mTimeBias;
}

bool Window::isMinimized() const {
	return gImpl->mMinimized;
}

bool Window::isActive() const {
	return gImpl->mActive;
}

int Window::getAndResetMouseWheel(){
	int t = gImpl->mMouseWheel;
	gImpl->mMouseWheel = 0;
	return t;
}

bool Window::isMainThread() {
	return ( GetCurrentThreadId() == gImpl->mThreadId );
}

void Window::Configuration::setWidth( int w ){
	gImpl->mWidth = w;
}

void Window::Configuration::setHeight( int h ){
	gImpl->mHeight = h;
}

void Window::Configuration::setTitle( const char* title ){
	gImpl->mTitle = title;
}

const char* Window::commandLineString() const {
	return gImpl->mCommandLine.c_str();
}

}

#define MAX_LOADSTRING 100

// グローバル変数:
HINSTANCE hInst;								// 現在のインターフェイス
TCHAR szTitle[MAX_LOADSTRING];					// タイトル バーのテキスト
TCHAR szWindowClass[MAX_LOADSTRING];			// メイン ウィンドウ クラス名

// このコード モジュールに含まれる関数の宣言を転送します:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);


int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: ここにコードを挿入してください。
	MSG msg;
	HACCEL hAccelTable;

	HRESULT hr = ::CoInitializeEx( NULL, COINIT_MULTITHREADED );
	//ウィンドウ状態保持クラス生成
	gImpl = NEW Impl();
	gImpl->mCommandLine = lpCmdLine;
	Window wc;
	Window::Configuration config;
	wc.configure( &config );

	memcpy(
		szTitle, 
		wc.title(), 
		wc.titleLength() ); 
	memcpy( szWindowClass, "Game Window Mashiro", sizeof("Game Window Mashiro"));

	MyRegisterClass( hInstance );

	// アプリケーションの初期化を実行します:
	if ( !InitInstance ( hInstance, nCmdShow ) ){
		return FALSE;
	}

	hAccelTable = LoadAccelerators( hInstance, MAKEINTRESOURCE( IDC_WINDOWCREATOR ) );

	timeBeginPeriod( 1 );
	HWND windowHandle = gImpl->mWindowHandle;
	///起動前の例外は継続不可能なので、メッセージボックスを出して終わらせる
	try{
		wc.start( windowHandle );
	}
	catch ( ... ){ //起動時の例外は無視できない。
		MessageBoxA( 
			windowHandle, 
			"致命的な問題があり、起動できません。申し訳ありませんが終了いたします", 
			"致命的エラー", 
			MB_OK | MB_ICONERROR );
		wc.requestEnd(); //終われ。
		wc.update(); //中で終了処理
		SAFE_DELETE( gImpl );
		timeEndPeriod( 1 );
		::CoUninitialize();
		return 0;
	}

	gImpl->mStarted = true; //ループ開始信号
	while ( true ){
		//メッセージがあるなら処理
		if ( PeekMessage( &msg, NULL, 0, 0, PM_NOREMOVE ) ){
			if ( GetMessage( &msg, NULL, 0, 0 ) ){
				if ( !TranslateAccelerator( msg.hwnd, hAccelTable, &msg ) ){
					TranslateMessage( &msg );
					DispatchMessage( &msg );
				}
			}else{
				break; //WM_QUITきました。ループ抜けます。
			}
		}
		//終わってればwindow破棄
		if ( gImpl->mEnded ){ //終わってなければ更新ループ
			if ( windowHandle ){ //もしかすると2回来るかもしれないので念のため
				DestroyWindow( windowHandle );
				windowHandle = 0;
			}
		}else{	//実行本体
			try{
				wc.update();
			}
			catch ( Mashiro::Exception e ){
				if ( e == Mashiro::EXCEPTION_EXIT ){ //終了要請例外の場合のみ終了
					MessageBoxA( 
						windowHandle, 
						"致命的な問題があり、継続できません。申し訳ありませんが終了いたします", 
						"致命的エラー", 
						MB_OK | MB_ICONERROR );
				}
			}
		}
	}
	SAFE_DELETE( gImpl );
	timeEndPeriod( 1 );
	::CoUninitialize();

	return (int) msg.wParam;
}



//
//  関数: MyRegisterClass()
//
//  目的: ウィンドウ クラスを登録します。
//
//  コメント:
//
//    この関数および使い方は、'RegisterClassEx' 関数が追加された
//    Windows 95 より前の Win32 システムと互換させる場合にのみ必要です。
//    アプリケーションが、関連付けられた
//    正しい形式の小さいアイコンを取得できるようにするには、
//    この関数を呼び出してください。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINDOWCREATOR));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_WINDOWCREATOR);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   関数: InitInstance(HINSTANCE, int)
//
//   目的: インスタンス ハンドルを保存して、メイン ウィンドウを作成します。
//
//   コメント:
//
//        この関数で、グローバル変数でインスタンス ハンドルを保存し、
//        メイン プログラム ウィンドウを作成および表示します。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	HWND hWnd;

	hInst = hInstance; // グローバル変数にインスタンス処理を格納します。
//----
	RECT rect;
	rect.left = 0;
	rect.top = 0;
	rect.right = gImpl->mWidth;
	rect.bottom = gImpl->mHeight;
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);
	rect.right -= rect.left;
	rect.bottom -= rect.top; //差分計算
	rect.left = 0;
	rect.top = 0;

//----

	HDC screen = GetDC( 0 );
	float dpiX = static_cast< FLOAT >( GetDeviceCaps( screen, LOGPIXELSX ) );
	float dpiY = static_cast< FLOAT >( GetDeviceCaps( screen, LOGPIXELSX ) );

	hWnd = CreateWindow( szWindowClass, szTitle, 
		WS_OVERLAPPEDWINDOW, 0, 0,
		static_cast< UINT >( rect.right * dpiX / 96.0f ),
		static_cast< UINT >( rect.bottom * dpiY / 96.0f ),
		NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   gImpl->mWindowHandle = hWnd;

   return TRUE;
}

//
//  関数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的:  メイン ウィンドウのメッセージを処理します。
//
//  WM_COMMAND	- アプリケーション メニューの処理
//  WM_PAINT	- メイン ウィンドウの描画
//  WM_DESTROY	- 中止メッセージを表示して戻る
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		break;
	case WM_CLOSE: //終了要請
		gImpl->mEndRequested = true;
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_ACTIVATE:
		if ( wParam & 0xffff0000 ){ //最小化されていればfalse
			gImpl->mActive = false;
		}else{
			gImpl->mActive = ( ( wParam & 0xffff ) != 0 );
		}
		break;
	case WM_MOUSEWHEEL:
		gImpl->mMouseWheel = ( static_cast< short >( HIWORD( wParam ) ) > 0 ) ? 1 : -1;
		break;
	case WM_SYSCOMMAND:
		if( wParam == SC_MAXIMIZE ){
			//なにもしない
		} else if ( wParam == SC_MINIMIZE ){
			gImpl->mMinimized = true;
			gImpl->mActive = false;
			DefWindowProc(hWnd, message, wParam, lParam);
		}else if ( wParam == SC_RESTORE ){
			gImpl->mMinimized = false;
			gImpl->mActive = true;
			DefWindowProc(hWnd, message, wParam, lParam);
		}else{
			DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}