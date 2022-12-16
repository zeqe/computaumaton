#include "uint.h"
#include "symbol.h"

// Char: a symbol of length 1 stored in an unsigned int. Valid formats:
// - MSB c LSB
// where c denotes a displayable character

unsigned int char_read(int in){
	if(!isgraph(in)){
		return 0;
	}
	
	return SYMBOL_SET(0,0,(unsigned char)in);
}

// Pair: a symbol of length 2 stored in an unsigned int. Valid formats:
// - MSB cc LSB
// - MSB 0c LSB
// where c denotes a displayable character and 0 denotes a 0 byte

unsigned int pair_symbol;
unsigned int pair_len;

void pair_read_init(){
	pair_symbol = 0;
	pair_len = 0;
}

unsigned int pair_read_update(int in){
	if(pair_len >= 2){
		// 2-symbol is already complete
		return 1;
	}
	
	switch(in){
	case '\t':
		// Attempt to complete 2-symbol with less than 2 characters
		switch(pair_len){
		case 0:
			// Cannot complete an empty 2-symbol
			return 0;
		case 1:
			// Complete as 2-symbol
			pair_len = 2;
			return 1;
		}
		
		break;
	case '\b':
		// Attempt to erase a character
		switch(pair_len){
		case 0:
			// Cannot erase from an empty 2-symbol
			return 0;
		case 1:
			// Erase last character
			--(pair_len);
			pair_symbol = SYMBOL_SET(pair_symbol,pair_len,0);
		}
		
		break;
	default:
		// Add a valid character
		if(!isgraph(in)){
			return 0;
		}
		
		pair_symbol = SYMBOL_SET(pair_symbol,pair_len,(unsigned char)in);
		++(pair_len);
		
		return (pair_len) >= 2;
	}
	
	return 0;
}

unsigned int pair_read_get(){
	return pair_symbol;
}

void pair_str(char strbuf[3],unsigned int pair){
	strbuf[0] = SYMBOL_GET(pair,0);
	strbuf[1] = SYMBOL_GET(pair,1);
	strbuf[2] = '\0';
}



unsigned int buf[5];
unsigned int buflen;

static void read_3uint_init_val(){
	if(!is_monad[buflen]){
		
	}
}

unsigned int read_3uint_init(unsigned int is_monad[3]){
	buflen = 0;
	read_3uint_init_val();
}

unsigned int read_3uint_handle_in(int in){
	if(buflen >= 3){
		return 1;
	}
	
	unsigned int new_val;
	
	if(is_monad[buflen] ? (new_val = char_read_handle_in(in)) : (pair_read_update(in) && new_val = pair_read_get())){
		buf[buflen] = new_val;
		++buflen;
	}
	
	if(buflen < 3){
		read_3uint_init_val();
	}
}











unsigned int read_symbol;
unsigned int read_bytes;
unsigned int read_max_bytes;

void sym1read_init(unsigned int max_bytes){
	read_symbol = 0;
	read_bytes = 0;
	read_max_bytes = max_bytes;
}

uint sym1read_update(int in){
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

uint sym1read_get(){
	return read_symbol;
}



struct list_1sym{
	unsigned int sym_size;
	struct list_1uint sym_list;
};

unsigned int read_Ntuple[N];
unsigned int read_symbols;
struct list_1sym *read_symbol_srcs[N];

void symNread_init(struct list_1sym *symbol_srcs[N]){
	memset(read_Ntuple,0,N * sizeof(unsigned int));
	read_symbols = 0;
	
	for(unsigned int i = 0;i < N;++i){
		read_symbol_srcs[i] = symbol_srcs[i];
	}
}

uint symNread_update(int in){
	if(read_symbols >= N){
		// Tuple is already complete
		return 1;
	}
	
	switch(in){
	case '<':
		// "Item backspace" operator
		
		if(read_Ntuple[read_symbols] != 0){
			// Clear current symbol ----------
			read_Ntuple[read_symbols] = 0;
			return 0;
			
		}else{
			// Remove last symbol ------------
			if(read_symbols == 0){
				// Cannot remove a symbol from an empty tuple
				return 0;
			}
			
			--read_symbols;
			return 0;
		}
		
		break;
	default:
		// Interpret input for individual tuple value
		
		if(!sym1read_update(in)){
			// Input interpreted, but value not completed
			return 0;
		}
		
		// Handle completed value
		read_Ntuple[read_symbols] = sym1read_get();
		++read_symbols;
		
		return read_symbols >= N;
	}
	
	return 0;
}