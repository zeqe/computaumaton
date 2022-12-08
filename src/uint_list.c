#include <string.h>

#include "uint_list.h"

void ul_init(struct uint_list *ul){
	ul->len = 0;
	ul->i = 0;
}

unsigned int ul_len(struct uint_list *ul){
	return ul->len;
}

void ul_add_after(struct uint_list *ul,unsigned int val){
	if(ul->len >= UINT_BLOCK_LEN){
		// List cannot exceed block size
		return;
	}
	
	// Insert at 0 if len = 0, otherwise insert at i + 1
	unsigned int insert_i = ul->i + (ul->len != 0);
	
	// Move elements at and after the insertion position
	memmove(
		ul->block + insert_i + 1,
		ul->block + insert_i + 0,
		(ul->len - insert_i) * sizeof(unsigned int)
	);
	
	++(ul->len);
	
	// Insert
	ul->block[insert_i] = val;
}

void ul_remove(struct uint_list *ul){
	if(ul->i >= ul->len){
		// There is no valid element to remove at the length of the list
		// This should handle attempts to remove from empty lists, since then len == i == 0
		return;
	}
	
	// Move elements to overwrite current index
	memmove(
		ul->block + ul->i + 0,
		ul->block + ul->i + 1,
		(ul->len - ul->i - 1) * sizeof(unsigned int)
	);
	
	--(ul->len);
}

static void ul_swap(struct uint_list *ul,unsigned int i,unsigned int j){
	unsigned int temp;
	
	temp         = ul->block[i];
	ul->block[i] = ul->block[j];
	ul->block[j] = temp;
}

void ul_val_forward(struct uint_list *ul){
	if(ul->i + 1 >= ul->len){
		// Cannot move last element any more forward
		return;
	}
	
	ul_swap(ul,ul->i,ul->i + 1);
}

void ul_val_backward(struct uint_list *ul){
	if(ul->i == 0){
		// Cannot move first element any more backward
		return;
	}
	
	ul_swap(ul,ul->i,ul->i - 1);
}

void ul_i_forward(struct uint_list *ul){
	if(ul->i + 1 >= ul->len){
		// Cannot go beyond the length of list
		return;
	}
	
	++(ul->i);
}

void ul_i_backward(struct uint_list *ul){
	if(ul->i == 0){
		// Cannot go beyond the beginning of the list
		return;
	}
	
	--(ul->i);
}

void ul_get(struct uint_list *ul,unsigned int *val){
	if(ul->i >= ul->len){
		// Cannot read a value from an element that is not a part of the list
		return;
	}
	
	(*val) = ul->block[ul->i];
}

void ul_set(struct uint_list *ul,unsigned int val){
	if(ul->i >= ul->len){
		// Cannot set a value to an element that is not a part of the list
		return;
	}
	
	ul->block[ul->i] = val;
}

void ul_forall(struct uint_list *ul,void (*f)(unsigned int,unsigned int,unsigned int)){ // f(val,index,is_current)
	for(unsigned int i = 0;i < ul->len;++i){
		(*f)(ul->data + i,i,ul->i == i);
	}
}