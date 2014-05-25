#ifndef INCLUDE_MASHIRO_THREAD_THREADING_H_
#define INCLUDE_MASHIRO_THREAD_THREADING_H_

namespace Mashiro {

namespace Threading {

class Thread {
public:
	//中身の処理
	virtual void operator()() = 0;
	//スレッド開始
	void start();
	//終了待ち
	void wait();
	//終わったかチェック
	bool isFinished();
	//直接は呼ばないよ
	Thread();
protected:
	~Thread();
private:
	class Impl;
	Impl* mImpl;
};

}
}

#endif
