#ifndef INCLUDE_MASHIRO_THREADING_EVENT_H_
#define INCLUDE_MASHIRO_THREADING_EVENT_H_

namespace Mashiro {

namespace Threading {

class Event {
public:
	static Event create( bool initState = false );
	//true待ち
	void wait() const;
	//trueにする
	void set();
	//falseにする
	void reset();
	//チェック
	bool isSet() const;

	Event();
	~Event();
	Event( const Event& );
	operator void*() const;
	Event& operator=( const Event& );
	void release();
	bool operator==( const Event& ) const;
	bool operator!=( const Event& ) const;
private:
	class Impl;
	Impl* mImpl;
};

}
}

#endif