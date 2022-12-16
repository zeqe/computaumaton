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



struct list_1sym{
	uint sym_size;
	struct list_1uint sym_list;
};

uint read_Ntuple[N];
uint read_symbols;
struct list_1sym *read_symbol_srcs[N];

void symNread_init(struct list_1sym *symbol_srcs[N]){
	memset(read_Ntuple,0,N * sizeof(uint));
	read_symbols = 0;
	
	for(uint i = 0;i < N;++i){
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
		
		if(!symread_update(in)){
			// Input interpreted, but value not completed
			return 0;
		}
		
		// Handle completed value
		read_Ntuple[read_symbols] = symread_get();
		++read_symbols;
		
		return read_symbols >= N;
	}
	
	return 0;
}