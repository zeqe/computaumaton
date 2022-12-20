#include <string.h>
#include <ctype.h>

#include "symbol.h"
#include "symread.h"

// symread ------------------------------||
static void symread_init(uint *buffer,uint *bytes){
	*buffer = 0;
	*bytes = 0;
}

static uint symread_update(uint *buffer,uint *bytes,uint max_bytes,int in){
	if(*bytes >= max_bytes){
		// Symbol is already complete
		return 1;
	}
	
	switch(in){
	case '\t':
		if(*bytes == 0){
			// Cannot complete an empty symbol
			return 0;
		}
		
		// Complete symbol
		*bytes = max_bytes;
		
		return 1;
		
	case '\b':
		if(*bytes == 0){
			// Cannot erase a byte from an empty symbol
			return 0;
		}
		
		// Erase a byte
		--(*bytes);
		*buffer = SYMBOL_SET(*buffer,*bytes,0);
		
		return 0;
		
	default:
		if(!isgraph(in)){
			// Cannot add a character that does not have a graphical representation
			return 0;
		}
		
		// Add a character-byte
		*buffer = SYMBOL_SET(*buffer,*bytes,(unsigned char)in);
		++(*bytes);
		
		return *bytes >= max_bytes;
	}
	
	return 0;
}

// symread(N) ---------------------------||
#define SYMREAD_DEFN(N)\
	\
	void symread(N) ## _init(struct symread(N) *read){\
		/* Set tuple read variables */\
		read->bytes = 0;\
		read->symbols = 0;\
		memset(read->buffer,0,N * sizeof(uint));\
		\
		/* Begin first symbol read */\
		if(read->symbols < N){\
			symread_init(&(read->buffer[read->symbols]),&(read->bytes));\
		}\
	}\
	\
	uint symread(N) ## _update(struct symread(N) *read,int in){\
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
			complete = symread_update(&(read->buffer[read->symbols]),&(read->bytes),read->sym_max_bytes[read->symbols],in);\
			\
			if(!complete){\
				/* Input interpreted, but symbol value not completed */\
				return 0;\
			}\
			\
			if(
				read->sym_srcs[read->symbols] != NULL &&
				!lu(1) ## _contains(read->sym_srcs[read->symbols],&(read->buffer[read->symbols]))
			){\
				/* Input interpreted and value completed, but the value is not a member of its source list */\
				read->buffer[read->symbols] = 0;\
				symread_init(&(read->buffer[read->symbols]),&(read->bytes));\
				\
				return 0;\
			}\
			\
			/* Handle completion of valid symbol */\
			++(read->symbols);\
			\
			if(read->symbols < N){\
				/* Prepare to read another symbol */\
				symread_init(&(read->buffer[read->symbols]),&(read->bytes));\
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
	const uint *symread(N) ## _get(struct symread(N) *read){\
		return read->buffer;\
	}

SYMREAD_DEFN(1)
SYMREAD_DEFN(3)
SYMREAD_DEFN(5)