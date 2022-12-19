#ifndef SET_INCLUDED
	#include "uint.h"
	#include "list.h"
	#include "symread.h"
	
	/*
	set(N): a list of symbol-tuples with its own symread(N) state
	
	set(N)_update() handles navigation and modification of the list.
	Any element added to the set will be checked for uniqueness; if it is not unique, it will be
	rejected.
	*/
	
	#define set(N) set ## N
	
	#define SET_DECL(N)\
		struct set(N){\
			struct symread(N) read;  \
			uint is_reading;         \
			\
			struct list_uint(N) list;\
		};\
		\
		uint set(N) ## _update(struct set(N) *set,int in);
	
	#define SET_INIT(MAX_BYTES,SRCS) {SYMREAD_INIT(MAX_BYTES,SRCS),0,LIST_INIT}
	
	SET_DECL(1)
	SET_DECL(3)
	SET_DECL(5)
	
	#define SET_INCLUDED
#endif