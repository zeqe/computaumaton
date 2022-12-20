#ifndef SYMREAD_INCLUDED
	#include "uint.h"
	#include "list.h"
	
	/*
	symread(N): constructs an N-tuple of symbols out of a sequence of input characters
	
	USE -------------------------------------||
	
	To construct a new value, a sequence of calls should look like:
	  s = _INIT(args)       // create a new buffer object, configured according to args
	  
	  _init(&s)             // begin a new read with an emptied buffer
	  _update(&s,in1) -> 0  // character interpreted, value construction not yet finished...
	  _update(&s,in2) -> 0  // another character interpreted, still not done...
	  _update(&s,in3) -> 0  // ...
	  _update(&s,in4) -> 1  // value completed!
	  _get(&s)              // retrieve the constructed value from the buffer
	
	Should _update() be called after a previous _update() call has returned 1, it will do nothing.
	  ...
	  _update(&s,in7) -> 0  // not done
	  _update(&s,in8) -> 1  // done
	  _update(&s,in9) -> 1  // does nothing; 'in9' is effectively ignored
	  ...
	Only calling _init() will make _update() receptive to inputs again.
	
	The value being constructed can be previewed with _get(), even if _update() has not yet
	returned 1:
	  ...
	  _update(&s,in5) -> 0
	  _get(&s)              // preview current state of buffer
	  _update(&s,in6) -> 1
	  _get(&s)              // retrieve buffer with knowledge that it is now complete, and will not
	                        // change until another call to _init() occurs
	
	CONFIGURATION ---------------------------||
	
	struct symread(N) s = SYMREAD_INIT(MAX_BYTES,SRCS);
	  uint MAX_BYTES[N]:
	    Specifies the maximum number of bytes allowed in the i-th symbol of the constructed tuple.
	    If the i-th symbol contains more bytes than MAX_BYTES[i], it will be rejected and wholly
	    reconstructed.
	    When _update() returns 1, it will be guaranteed that the number of bytes in the i-th symbol
	    will be > 0, unless MAX_BYTES[i] == 0, in which case it will == 0.
	  
	  struct list_uint(1) *SRCS[N]:
	    Specifies the list to which the i-th symbol of the constructed tuple must be a part of.
	    If the i-th symbol is not contained in SRCS[i], it will be rejected and wholly
	    reconstructed.
	    If SRCS[i] == NULL, the i-th symbol will not be checked for membership in a list. It will
	    never be rejected on this basis.
	*/
	
	#define symread(N) symread ## N
	
	#define SYMREAD_DECL(N)\
		struct symread(N){\
			/* Configuration */            \
			uint sym_max_bytes[N];         \
			struct list_uint(1) *sym_srcs[N];\
			\
			/* Run-time data */\
			uint bytes         \
			uint symbols;      \
			\
			uint buffer[N];    \
		};\
		\
		      void  symread(N) ## _init  (struct symread(N) *read);       \
		      uint  symread(N) ## _update(struct symread(N) *read,int in);\
		const uint *symread(N) ## _get   (struct symread(N) *read);
	
	#define SYMREAD_INIT(MAX_BYTES,SRCS) {MAX_BYTES,SRCS,0,0}
	
	SYMREAD_DECL(1)
	SYMREAD_DECL(3)
	SYMREAD_DECL(5)
	
	#define SYMREAD_INCLUDED
#endif