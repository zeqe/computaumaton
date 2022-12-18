#ifndef SYMREAD_INCLUDED
	#include "uint.h"
	#include "list.h"
	
	/*
	symread:  constructs a symbol out of a sequence of input characters
	symNread: constructs an N-tuple of symbols out of a sequence of input characters
	
	USE -------------------------------------||
	
	To construct a new value, a sequence of calls should look like:
	  _init()            // begin a new read with an emptied buffer
	  _update(in1) -> 0  // character interpreted, value construction not yet finished...
	  _update(in2) -> 0  // another character interpreted, still not done...
	  _update(in3) -> 0  // ...
	  _update(in4) -> 1  // value completed!
	  _get()             // retrieve the constructed value from the buffer
	
	Should _update() be called after a previous _update() call has returned 1, it will do nothing.
	  ...
	  _update(in7) -> 0  // not done
	  _update(in8) -> 1  // done
	  _update(in9) -> 1  // does nothing; 'in9' is effectively ignored
	  ...
	Only calling _init() will make _update() receptive to inputs again.
	
	The value being constructed can be previewed with _get(), even if _update() has not yet
	returned 1:
	  ...
	  _update(in5) -> 0
	  _get()             // preview current state of buffer
	  _update(in6) -> 1
	  _get()             // retrieve buffer with knowledge that it is now complete, and will not
	                     // change until another call to _init() occurs
	
	EXPECTATIONS-----------------------------||
	
	symread:
	  When _init(max_bytes) is called, it sets the maximum number of bytes allowed in the symbol
	  currently being constructed.
	  Further calls to _update() will ensure that (0 < symbol_bytes <= max_bytes) when _update()
	  returns 1, unless max_bytes == 0, in which case (0 == symbol_bytes == max_bytes).
	
	symNread:
	  When _init(symbol_srcs) is called, it determines the "source list" for each symbol in the
	  tuple. That is, if the i-th symbol of the tuple is not contained in symbol_srcs[i], it will
	  be rejected, and the i-th symbol will be wholly re-constructed.
	  symbol_srcs cannot have any NULL elements; symNread_ functions use
	  symbol_srcs[i]->sym_max_bytes to determine the maximum byte size of each constituent symbol.
	
	Note that symNread_ functions internally use symread_ functions to read individual symbols.
	Thus, symNread_ function calls cannot be mixed with one another, nor can they be mixed with
	symread_ calls.
	*/
	
	// symread
	void symread_init(uint max_bytes);
	uint symread_update(int in);
	uint symread_get();
	
	// symNread
	void sym1read_init(struct list_sym *symbol_srcs[1]);
	uint sym1read_update(int in);
	void sym1read_get(uint buffer[1]);
	
	void sym3read_init(struct list_sym *symbol_srcs[3]);
	uint sym3read_update(int in);
	void sym3read_get(uint buffer[3]);
	
	void sym5read_init(struct list_sym *symbol_srcs[5]);
	uint sym5read_update(int in);
	void sym5read_get(uint buffer[5]);
	
	#define SYMREAD_INCLUDED
#endif