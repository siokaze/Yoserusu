#ifndef INCLUDE_RELELASE_H_
#define INCLUDE_RELELASE_H_

namespace Mashiro{

namespace Graphics {

template<class Interface>
inline void
SafeRelease( Interface **ppInterfaceToRelease ){
	if (*ppInterfaceToRelease != NULL)
	{
		(*ppInterfaceToRelease)->Release();
		(*ppInterfaceToRelease) = NULL;
	}
}

}

}

#endif