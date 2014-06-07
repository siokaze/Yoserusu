#ifndef INCLUDE_UTIL_H_
#define INCLUDE_UTIL_H_

#include "boost/tuple/tuple.hpp"

template< int N >
struct IntType{
	static const int Value = N;
};

using boost::tuples::length;

template< typename tuple_t, typename F, int index >
void tuple_for_each_impl( tuple_t& tuple, F& func, IntType<index> ){
	func( tuple.get< index >() );
	tuple_for_each_impl( tuple, func, IntType< index + 1 >() );
}

template< typename tuple_t, typename F >
void tuple_for_each_impl( tuple_t&, F&, IntType< length<tuple_t>::value > ){}

template< typename tuple_t, typename F >
void tupe_for_each( tuple_t& tuple, F func ){
	tuple_for_each_impl( tuple, func, IntType<0>() );
}


#endif