#include <limits.h>
#include <string.h>

#include "uint_block_list.h"

void ubl_init(struct uint_block_list *ubl){
	ubl->len = 0;
	ubl->i = 0;
}

static int ubl_val_unique(struct uint_block_list *ubl,unsigned int val){
	// Checks to see if the supplied value is already present in the uint_block_list;
	// returns 0 if so, to indiciate non-uniqueness, returns 1 if not, to indicate uniqueness
	
	for(unsigned int i = 0;i < ubl->len;++i){
		if(ubl->block[i] == val){
			return 0;
		}
	}
	
	return 1;
}

void ubl_add(struct uint_block_list *ubl,unsigned int val){
	if(ubl->len >= UINT_BLOCK_SIZE){
		// List cannot exceed block size
		return;
	}
	
	if(!ubl_val_unique(ubl,val)){
		// Duplicate values are not allowed, in the spirit of emulating mathematical sets
		return;
	}
	
	// Move elements after the insertion position
	memmove(ubl->block + ubl->i + 1,ubl->block + ubl->i,(ubl->len - ubl->i) * sizeof(unsigned int));
	++(ubl->len);
	
	// Insert
	ubl->block[ubl->i] = val;
}

void ubl_remove(struct uint_block_list *ubl){
	if(ubl->i == ubl->len){
		// There is no valid element to remove at the length of the list
		// This should handle attempts to remove from empty lists, since then len == i == 0
		return;
	}
	
	// Move elements to overwrite current index
	memmove(ubl->block + ubl->i,ubl->block + ubl->i + 1,(ubl->len - ubl->i - 1) * sizeof(unsigned int));
	--(ubl->len);
}

void ubl_forward(struct uint_block_list *ubl){
	if(ubl->i >= ubl->len){
		// Cannot go beyond the length of list
		return;
	}
	
	++(ubl->i);
}

void ubl_backward(struct uint_block_list *ubl){
	if(ubl->i == 0){
		// Cannot go beyond the beginning of the list
		return;
	}
	
	--(ubl->i);
}

unsigned int ubl_get(struct uint_block_list *ubl){
	if(ubl->i >= ubl->len){
		// Cannot read a value from an element that is not a part of the list
		return UINT_MAX;
	}
	
	return ubl->block[ubl->i];
}

void ubl_set(struct uint_block_list *ubl,unsigned int val){
	if(ubl->i >= ubl->len){
		// Cannot set a value to an element that is not a part of the list
		return;
	}
	
	if(!ubl_val_unique(ubl,val)){
		// Duplicate values are not allowed, in the spirit of emulating mathematical sets
		return;
	}
	
	ubl->block[ubl->i] = val;
}