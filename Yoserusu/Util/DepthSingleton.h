#ifndef INCLUDE_DEPTH_SINGLETON_H_
#define INCLUDE_DEPTH_SINGLETON_H_

class DepthSingleton {
public:
	static DepthSingleton* instance();
	static void create();
	static void destory();

	//深度値セット
	void setDepthMin( float i );
	void setDepthMax( float i );
	//深度値ゲット
	int getDepthMin() const;
	int getDepthMax() const;
private:
	DepthSingleton();
	~DepthSingleton();
	static DepthSingleton* mInstance;

	//深度値
	float mDepthMax;
	float mDepthMin;
};

#endif