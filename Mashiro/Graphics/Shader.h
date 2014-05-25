#ifndef INCLUDE_MASHIRO_SHADER_SHADER_H_
#define INCLUDE_MASHIRO_SHADER_SHADER_H_

#include "Mashiro/Graphics/Enum.h"

namespace Mashiro{
namespace Graphics{

class Shader {
public:
public:
	static Shader create( ShaderType shaderType );
	bool lock( void** );
	void unlock();

	//à»â∫ÉÜÅ[ÉUÇÕãCÇ…Ç∑ÇÈÇ»
	Shader();
	~Shader();
	Shader( const Shader& );
	operator void*() const;
	Shader& operator=( const Shader& );
	void release();
	bool operator==( const Shader& ) const;
	bool operator!=( const Shader& ) const;
	class Impl;
	Shader( const Impl* );
private:
	friend class Manager;
	Impl* mImpl;
};

}
}

#endif