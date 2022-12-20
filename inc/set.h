#ifndef SET_INCLUDED
	#include "uint.h"
	#include "list.h"
	#include "symread.h"
	
	/*
	set(N): a list of unique symbol N-tuples with its own symread(N) state
	
	_update():
	- handles navigation and modification of the list as dictated by a sequence of input characters
	- ensures that elements added to the list are unique (duplicates will be rejected)
	- will return 0 if the operation performed is incomplete, or 1 if it is complete
	    ...
	    _update() -> 1 // first operation complete 
	    _update() -> 0 // second operation not complete
	    _update() -> 0 // still not complete
	    _update() -> 0 // ...
	    _update() -> 1 // second operation complete!
	    _update() -> 1 // third operation complete
	    _update() -> 1 // fourth operation complete
	    ...
	*/
	
	#define set(N) set ## N
	
	#define SET_DECL(N)\
		struct set(N){\
			uint is_reading;         \
			struct symread(N) read;  \
			\
			struct list_uint(N) list;\
		};\
		\
		uint set(N) ## _update(struct set(N) *set,int in);
	
	#define SET_INIT(MAX_BYTES,SRCS) {0,SYMREAD_INIT(MAX_BYTES,SRCS),LIST_INIT}
	
	SET_DECL(1)
	SET_DECL(3)
	SET_DECL(5)
	
	#define SET_INCLUDED
#endif