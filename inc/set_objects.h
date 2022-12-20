#ifndef SET_OBJECTS_INCLUDED
	#include "uint.h"
	#include "symread.h"
	#include "list.h"
	
	/*
	set objects: element, set
	
	_update():
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
	
	// --------------------------------------||
	
	/*
	element(N): a symbol N-tuple with its own symread(N) state
	
	_update():
	- handles modification of the element as dictated by a sequence of input characters
	*/
	
	#define element(N) element ## N
	
	#define ELEMENT_DECL(N)\
		struct element(N){\
			uint is_reading;       \
			struct symread(N) read;\
			\
			uint is_specified;     \
			uint value[N];         \
		};\
		\
		uint element(N) ## _update(struct element(N) *element,int in);
	
	#define ELEMENT_INIT(MAX_BYTES,SRCS) {0,SYMREAD_INIT(MAX_BYTES,SRCS),0}
	
	ELEMENT_DECL(1)
	ELEMENT_DECL(3)
	ELEMENT_DECL(5)
	
	/*
	set(N): a list of unique symbol N-tuples with its own symread(N) state
	
	_update():
	- handles navigation and modification of the list as dictated by a sequence of input characters
	- ensures that elements added to the list are unique (duplicates will be rejected)
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
	
	#define SET_OBJECTS_INCLUDED
#endif