#include <string.h>

#include "symbol.h"
#include "symread.h"

uint read_symbol;
uint read_bytes;
uint read_max_bytes;

void symread_init(uint max_bytes){
	read_symbol = 0;
	read_bytes = 0;
	read_max_bytes = max_bytes;
}

uint symread_update(int in){
	if(read_bytes >= read_max_bytes){
		// Symbol is already complete
		return 1;
	}
	
	switch(in){
	case '\t':
		if(read_bytes == 0){
			// Cannot complete an empty symbol
			return 0;
		}
		
		// Complete symbol
		read_bytes = read_max_bytes;
		
		return 1;
		
	case '\b':
		if(read_bytes == 0){
			// Cannot erase a byte from an empty symbol
			return 0;
		}
		
		// Erase a byte
		--read_bytes;
		read_symbol = SYMBOL_SET(read_symbol,read_bytes,0);
		
		return 0;
		
	default:
		if(!isgraph(in)){
			// Cannot add a character that does not have a graphical representation
			return 0;
		}
		
		// Add a character-byte
		read_symbol = SYMBOL_SET(read_symbol,read_bytes,(unsigned char)in);
		++read_bytes;
		
		return read_bytes >= read_max_bytes;
	}
	
	return 0;
}

uint symread_get(){
	return read_symbol;
}

#define SYMNREAD_TUPLE(N) read ## N ## _tuple
#define SYMNREAD_SYMBOLS(N) read ## N ## _symbols
#define SYMNREAD_SYMBOL_SRCS(N) read ## N ## _symbol_srcs

#define SYMNREAD_(N)\
	uint SYMNREAD_TUPLE(N)[N];                  \
	uint SYMNREAD_SYMBOLS(N);                   \
	struct list_sym *SYMNREAD_SYMBOL_SRCS(N)[N];\
	\
	void sym ## N ## read_init(struct list_sym *symbol_srcs[N]){\
		/* Set tuple read variables */\
		memset(SYMNREAD_TUPLE(N),0,N * sizeof(uint));\
		SYMNREAD_SYMBOLS(N) = 0;\
		\
		for(uint i = 0;i < N;++i){\
			SYMNREAD_SYMBOL_SRCS(N)[i] = symbol_srcs[i];\
		}\
		\
		/* Begin first symbol read */\
		if(SYMNREAD_SYMBOLS(N) < N){\
			symread_init(SYMNREAD_SYMBOL_SRCS(N)[SYMNREAD_SYMBOLS(N)]->sym_max_bytes);\
		}\
	}\
	\
	uint sym ## N ## read_update(int in){\
		if(SYMNREAD_SYMBOLS(N) >= N){\
			/* Tuple is already complete */\
			return 1;\
		}\
		\
		uint complete = 0;\
		\
		switch(in){\
		case '<':\
			/* "Item backspace" operator */\
			\
			if(SYMNREAD_TUPLE(N)[SYMNREAD_SYMBOLS(N)] != 0){\
				/* Clear current symbol ---------- */\
				SYMNREAD_TUPLE(N)[SYMNREAD_SYMBOLS(N)] = 0;\
				return 0;\
				\
			}else{\
				/* Remove last symbol ------------ */\
				if(SYMNREAD_SYMBOLS(N) == 0){\
					/* Cannot remove a symbol from an empty tuple */\
					return 0;\
				}\
				\
				--SYMNREAD_SYMBOLS(N);\
				return 0;\
			}\
			\
			break;\
		default:\
			/* Interpret input for individual tuple value */\
			complete = symread_update(in);\
			SYMNREAD_TUPLE(N)[SYMNREAD_SYMBOLS(N)] = symread_get();\
			\
			if(!complete){\
				/* Input interpreted, but symbol value not completed */\
				return 0;\
			}\
			\
			if(!lu1_contains(&(SYMNREAD_SYMBOL_SRCS(N)[SYMNREAD_SYMBOLS(N)]->sym_list),&(SYMNREAD_TUPLE(N)[SYMNREAD_SYMBOLS(N)]))){\
				/* Input interpreted and value completed, but the value is not a member of its source list */\
				SYMNREAD_TUPLE(N)[SYMNREAD_SYMBOLS(N)] = 0;\
				symread_init(SYMNREAD_SYMBOL_SRCS(N)[SYMNREAD_SYMBOLS(N)]->sym_max_bytes);\
				\
				return 0;\
			}\
			\
			/* Handle completion of valid symbol */\
			++SYMNREAD_SYMBOLS(N);\
			\
			if(SYMNREAD_SYMBOLS(N) < N){\
				/* Prepare to read another symbol */\
				symread_init(SYMNREAD_SYMBOL_SRCS(N)[SYMNREAD_SYMBOLS(N)]->sym_max_bytes);\
				\
				return 0;\
			}else{\
				/* Tuple value complete */\
				return 1;\
			}\
		}\
		\
		return 0;\
	}\
	\
	void sym ## N ## read_get(uint buffer[N]){\
		/* Copy tuple to buffer */\
		for(uint i = 0;i < N;++i){\
			buffer[i] = SYMNREAD_TUPLE(N)[i];\
		}\
	}

SYMNREAD_(1)
SYMNREAD_(3)
SYMNREAD_(5)