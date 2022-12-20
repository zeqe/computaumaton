#ifndef LIST_INCLUDED
	#include "uint.h"
	
	#define LIST_BLOCK_LEN 256
	
	/*
	list:
	
	ILLUSTRATION-----------------------------||
	
	|                     [---i=1---]            |len=3
	|                     \         /            |
	| block: [(#,#,#,#,#),(#,#,#,#,#),(#,#,#,#,#),(_,_,_,_,_),(_,_,_,_,_),...]
	
	Each value '#' is of type T.
	In this example, N = 5.
	
	'i' is the current position in the list.
	'block' is the statically allocated space for the list to hold its items; it has enough space
	to contain LIST_BLOCK_LEN N-tuples.
	
	Thus:
	  0 <= i <  len      <= LIST_BLOCK_LEN
	Unless len == 0, then:
	  0 <= i <= len == 0 <= LIST_BLOCK_LEN
	
	NOTES -----------------------------------||
	
	NAME determines the name of the resultant list struct, and PREFIX determines the prefix
	prepended to the generated functions to uniquely discern them from those corresponding to other
	list types.
	
	Documentation for the iterative functions:
	  _forall:   f is called as f(T val[N],uint index)
	  
	  _removeif: f is called as f(T val[N])
	             if f returns 0, the element will be kept.
	             if f returns != 0, the element will be removed.
	             
	  _contains: will return 1 if the list contains val, 0 otherwise
	*/
	
	#define LIST_DECL(T,N,NAME,PREFIX)\
		struct NAME{\
			uint len;                   \
			uint i;                     \
			\
			T block[N * LIST_BLOCK_LEN];\
		};\
		\
		uint PREFIX ## _len(struct NAME *l);\
		uint PREFIX ## _i  (struct NAME *l);\
		\
		void PREFIX ## _add_after(struct NAME *l,T val[N]);\
		void PREFIX ## _remove   (struct NAME *l);         \
		\
		void PREFIX ## _val_forward (struct NAME *l);\
		void PREFIX ## _val_backward(struct NAME *l);\
		\
		void PREFIX ## _i_forward (struct NAME *l);\
		void PREFIX ## _i_backward(struct NAME *l);\
		\
		void PREFIX ## _get(struct NAME *l,T val[N]);\
		void PREFIX ## _set(struct NAME *l,T val[N]);\
		\
		void PREFIX ## _forall  (struct NAME *l,void (*f)(T [N],uint));\
		void PREFIX ## _removeif(struct NAME *l,uint (*f)(T [N]));     \
		uint PREFIX ## _contains(struct NAME *l,T val[N]);
	
	#define LIST_INIT {0,0}
	
	// uint-tuple lists ---------------------||
	#define list_uint(N) list_uint ## N
	#define lu(N) lu ## N
	
	LIST_DECL(uint,1,list_uint(1),lu(1))
	LIST_DECL(uint,3,list_uint(3),lu(3))
	LIST_DECL(uint,5,list_uint(5),lu(5))
	
	#define LIST_INCLUDED
#endif