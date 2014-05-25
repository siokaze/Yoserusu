#ifndef INCLUDED_MASHIRO_THREADING_MANAGER_H
#define INCLUDED_MASHIRO_THREADING_MANAGER_H

namespace Mashiro{
namespace Threading{

class Manager{
public:
	Manager();
	static Manager instance();

	//コア数取得
	int getCoreNumber() const;

	//以下ユーザが呼ぶことはない
	static void create( int additionalThreadNumber );
	static void destroy();
};

}
}

#endif