#ifndef INCLUDE_WINDOW
#define INCLUDE_WINDOW

namespace Mashiro{

class Window{
public:
	class Configuration{
	public:
		void setWidth( int width );
		void setHeight( int height );
		void setTitle( const char* title );
	};
	Window();
	void start( void* windowHandle );
	void configure( Configuration* );
	void update();

	int width() const;
	int height() const;
	bool isWindowScreen() const;
	bool isMinimized() const;
	bool isActive() const;
	void enableWindowScreen( bool );
	unsigned time() const;
	int titleLength() const;
	const char* title() const;
	//コマンドライン文字列取得
	const char* commandLineString() const;

	void requestEnd();
	void end();
	bool isEndRequested() const;
	int getAndResetMouseWheel();
	static bool isMainThread();

};

}

#endif
