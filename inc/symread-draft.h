#ifndef SYMREAD_INCLUDED
	#include "uint.h"
	#include "symbol.h"
	
	/*
	symread: constructs a symbol value out of a sequence of input characters
	
	USE ------------||
	
	A sequence of calls to symread_ functions should look like:
	  symread_init()            // begin a new read, clear previous read's state
	  symread_update(in1) -> 0  // character interpreted, symbol construction not yet finished...
	  symread_update(in2) -> 0  // another character interpreted, still not done...
	  symread_update(in3) -> 0  // ...
	  symread_update(in4) -> 1  // symbol completed!
	  symread_get()             // retrieve the constructed symbol
	
	Should symread_update() be called after a previous call has returned 1, it will do nothing.
	  ...
	  symread_update(in7) -> 0  // not done
	  symread_update(in8) -> 1  // done
	  symread_update(in9) -> 1  // indicates continued completion; 'in9' does not affect symread_ state
	  ...
	Only calling symread_init() will make symread_update() receptive to inputs again.
	
	The symbol being constructed can be previewed with symread_get(), even if symread_update() has not
	returned 1:
	  ...
	  symread_update(in5) -> 0
	  symread_get()             // preview current state of symbol buffer
	  symread_update(in6) -> 1
	  symread_get()             // retrieve symbol buffer with knowledge that it is now complete, and
	                            // will not change until another call to symread_init() occurs
	
	FORMAT ---------||
	
	When a read is started with symread_init(max_bytes), further calls to symread_update() will ensure
	that the symbol being constructed will not have more than 'max_bytes' bytes packed into it.
	
	When symread_update() returns 1, it is guaranteed that the symbol constructed is not empty (ie, it
	will contain more than 0 bytes, unless max_bytes == 0).
	
	Thus:
	  0  < bytes <= max_bytes
	Unless max_bytes == 0, then:
	  0 == bytes == max_bytes
	*/
	
	void symread_init(uint max_bytes);
	uint symread_update(int in);
	uint symread_get();
	
	/*
	symNread: constructs a tuple of N symbol elements out of a sequence of input characters
	
	USE ------------||
	
	The lifetime requirements and expectations for symNread_ functions are the same as for symread_
	functions.
	
	The only difference is symNread_init():
	You do not specify the maximum length, since tuple length is already specified to be N.
	Instead, you provide an array to parent lists for each symbol value.
	
	
	One caveat: symNread_ functions will internally read the individual symbol values for the tuple
	using symread_ functions. Therefore, symNread_ calls cannot be mixed with one another, nor can they
	be mixed with symread_ calls, unless state disruption is desired.
	*/
	
	void sym1read_init(struct list_sym symbol_srcs[1]);
	uint sym1read_update(int in);
	void sym1read_get(uint buffer[1]);
	
	void sym3read_init(struct list_sym symbol_srcs[3]);
	uint sym3read_update(int in);
	void sym3read_get(uint buffer[3]);
	
	void sym5read_init(struct list_sym symbol_srcs[5]);
	uint sym5read_update(int in);
	void sym5read_get(uint buffer[5]);
	
	#define SYMREAD_INCLUDED
#endif