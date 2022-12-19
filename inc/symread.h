#ifndef SYMREAD_INCLUDED
	#include "uint.h"
	#include "list.h"
	
	/*
	symread_:  constructs a symbol out of a sequence of input characters
	symNread_: constructs an N-tuple of symbols out of a sequence of input characters
	
	USE -------------------------------------||
	
	To construct a new value, a sequence of calls should look like:
	  s = _INIT(args)       // create a new symread/symNread buffer object,
	                        // configured according to args
	  
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
	
	struct symread s = SYMREAD_INIT(MAX_BYTES);
	  uint MAX_BYTES:
	    Specifies the maximum number of bytes allowed in any symbol constructed with s.
	    When _update(&s,#) returns 1, it will be guaranteed that (0 < symbol_bytes <= MAX_BYTES),
	    unless MAX_BYTES == 0, in which case (0 == symbol_bytes == MAX_BYTES).
	
	struct symNread s = SYMNREAD_INIT(MAX_BYTES,SRCS);
	  uint MAX_BYTES[N]:
	    Specifies the maximum number of bytes allowed in the i-th symbol of the constructed tuple.
	    If the i-th symbol contains more bytes than MAX_BYTES[i], it will be rejected and wholly
	    reconstructed.
	  
	  struct SRC_LIST_NAME *SRCS[N]:
	    Specifies the list to which the i-th symbol of the constructed tuple must be a part of.
	    If the i-th symbol is not contained in SRCS[i], it will be rejected and wholly
	    reconstructed.
	    If SRCS[i] == NULL, the i-th symbol will not be checked for membership in a list. It will
	    never be rejected on this basis.
	*/
	
	// symread ------------------------------||
	struct symread{
		// Configuration
		uint max_bytes;
		
		// Run-time data
		uint bytes;
		uint buffer;
	};
	
	#define SYMREAD_INIT(MAX_BYTES) {MAX_BYTES,0}
	
	void symread_init  (struct symread *read);
	uint symread_update(struct symread *read,int in);
	uint symread_get   (struct symread *read);
	
	// symNread -----------------------------||
	#define SYMNREAD_NAME(N) sym ## N ## read
	
	#define SYMNREAD_DECL(N,SRC_LIST_NAME,SRC_LIST_PREFIX)\
		struct SYMNREAD_NAME(N){\
			/* Configuration */            \
			uint sym_max_bytes[N];         \
			struct SRC_LIST_NAME *sym_srcs[N];\
			\
			/* Run-time data */\
			uint symbols;      \
			uint buffer[N];    \
			\
			struct symread sym_read;\
		};\
		\
		      void  SYMNREAD_NAME(N) ## _init  (struct SYMNREAD_NAME(N) *read);       \
		      uint  SYMNREAD_NAME(N) ## _update(struct SYMNREAD_NAME(N) *read,int in);\
		const uint *SYMNREAD_NAME(N) ## _get   (struct SYMNREAD_NAME(N) *read);
	
	#define SYMNREAD_INIT(MAX_BYTES,SRCS) {MAX_BYTES,SRCS,0}
	
	SYMNREAD_DECL(1,list_uint1,lu1)
	SYMNREAD_DECL(3,list_uint1,lu1)
	SYMNREAD_DECL(5,list_uint1,lu1)
	
	#define SYMREAD_INCLUDED
#endif