#include <string.h>
#include <ctype.h>

#include "symbol.h"
#include "symread.h"

void symread_init(struct symread *read){
	read->buffer = 0;
	read->bytes = 0;
}

uint symread_update(struct symread *read,int in){
	if(read->bytes >= read->max_bytes){
		// Symbol is already complete
		return 1;
	}
	
	switch(in){
	case '\t':
		if(read->bytes == 0){
			// Cannot complete an empty symbol
			return 0;
		}
		
		// Complete symbol
		read->bytes = read->max_bytes;
		
		return 1;
		
	case '\b':
		if(read->bytes == 0){
			// Cannot erase a byte from an empty symbol
			return 0;
		}
		
		// Erase a byte
		--(read->bytes);
		read->buffer = SYMBOL_SET(read->buffer,read->bytes,0);
		
		return 0;
		
	default:
		if(!isgraph(in)){
			// Cannot add a character that does not have a graphical representation
			return 0;
		}
		
		// Add a character-byte
		read->buffer = SYMBOL_SET(read->buffer,read->bytes,(unsigned char)in);
		++(read->bytes);
		
		return read->bytes >= read->max_bytes;
	}
	
	return 0;
}

uint symread_get(struct symread *read){
	return read->buffer;
}

#define SYMNREAD_DEFN(N,SRC_LIST_NAME,SRC_LIST_PREFIX)\
	\
	static void SYMNREAD_NAME(N) ## _begin_symbol(struct SYMNREAD_NAME(N) *read){\
		read->sym_read.max_bytes = read->sym_max_bytes[read->symbols];\
		symread_init(&(read->sym_read));\
	}\
	\
	void SYMNREAD_NAME(N) ## _init(struct SYMNREAD_NAME(N) *read){\
		/* Set tuple read variables */\
		memset(read->buffer,0,N * sizeof(uint));\
		read->symbols = 0;\
		\
		/* Begin first symbol read */\
		if(read->symbols < N){\
			SYMNREAD_NAME(N) ## _begin_symbol(read);\
		}\
	}\
	\
	uint SYMNREAD_NAME(N) ## _update(struct SYMNREAD_NAME(N) *read,int in){\
		if(read->symbols >= N){\
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
			if(read->buffer[read->symbols] != 0){\
				/* Clear current symbol ---------- */\
				read->buffer[read->symbols] = 0;\
				return 0;\
				\
			}else{\
				/* Remove last symbol ------------ */\
				if(read->symbols == 0){\
					/* Cannot remove a symbol from an empty tuple */\
					return 0;\
				}\
				\
				--(read->symbols);\
				return 0;\
			}\
			\
			break;\
		default:\
			/* Interpret input for individual tuple value */\
			complete = symread_update(&(read->sym_read),in);\
			read->buffer[read->symbols] = symread_get(&(read->sym_read));\
			\
			if(!complete){\
				/* Input interpreted, but symbol value not completed */\
				return 0;\
			}\
			\
			if(
				read->sym_srcs[read->symbols] != NULL &&
				!SRC_LIST_PREFIX ## _contains(read->sym_srcs[read->symbols],&(read->buffer[read->symbols]))
			){\
				/* Input interpreted and value completed, but the value is not a member of its source list */\
				read->buffer[read->symbols] = 0;\
				SYMNREAD_NAME(N) ## _begin_symbol(read);\
				\
				return 0;\
			}\
			\
			/* Handle completion of valid symbol */\
			++(read->symbols);\
			\
			if(read->symbols < N){\
				/* Prepare to read another symbol */\
				SYMNREAD_NAME(N) ## _begin_symbol(read);\
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
	const uint *SYMNREAD_NAME(N) ## _get(struct SYMNREAD_NAME(N) *read){\
		return read->buffer;\
	}

SYMNREAD_DEFN(1,list_uint1,lu1)
SYMNREAD_DEFN(3,list_uint1,lu1)
SYMNREAD_DEFN(5,list_uint1,lu1)