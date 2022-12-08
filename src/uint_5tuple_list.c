#include <string.h>

#include "uint_5tuple_list.h"

void u5l_init(struct uint_5tuple_list *u5l){
	u5l->len = 0;
	u5l->i = 0;
}

unsigned int u5l_len(struct uint_5tuple_list *u5l){
	return u5l->len;
}

void u5l_add_after(struct uint_5tuple_list *u5l,unsigned int val[5]){
	if(u5l->len >= UINT_5TUPLE_BLOCK_LEN){
		// List cannot exceed block size
		return;
	}
	
	// Insert at 0 if len = 0, otherwise insert at i + 1
	unsigned int insert_i = u5l->i + (u5l->len != 0);
	
	// Move elements at and after the insertion position
	memmove(
		u5l->block + 5 * (insert_i + 1),
		u5l->block + 5 * (insert_i + 0),
		5 * (u5l->len - insert_i) * sizeof(unsigned int)
	);
	
	++(u5l->len);
	
	// Insert
	u5l->block[5 * insert_i + 0] = val[0];
	u5l->block[5 * insert_i + 1] = val[1];
	u5l->block[5 * insert_i + 2] = val[2];
	u5l->block[5 * insert_i + 3] = val[3];
	u5l->block[5 * insert_i + 4] = val[4];
}

void u5l_remove(struct uint_5tuple_list *u5l){
	if(u5l->i >= u5l->len){
		// There is no valid element to remove at the length of the list
		// This should handle attempts to remove from empty lists, since then len == i == 0
		return;
	}
	
	// Move elements to overwrite current index
	memmove(
		u5l->block + 5 * (u5l->i + 0),
		u5l->block + 5 * (u5l->i + 1),
		5 * (u5l->len - u5l->i - 1) * sizeof(unsigned int)
	);
	
	--(u5l->len);
}

static void u5l_swap(struct uint_5tuple_list *u5l,unsigned int i,unsigned int j){
	unsigned int temp[5];
	
	memcpy(temp              ,u5l->block + 5 * i,5 * sizeof(unsigned int));
	memcpy(u5l->block + 5 * i,u5l->block + 5 * j,5 * sizeof(unsigned int));
	memcpy(u5l->block + 5 * j,temp              ,5 * sizeof(unsigned int));
}

void u5l_val_forward(struct uint_5tuple_list *u5l){
	if(u5l->i + 1 >= u5l->len){
		// Cannot move last element any more forward
		return;
	}
	
	u5l_swap(u5l,u5l->i,u5l->i + 1);
}

void u5l_val_backward(struct uint_5tuple_list *u5l){
	if(u5l->i == 0){
		// Cannot move first element any more backward
		return;
	}
	
	u5l_swap(u5l,u5l->i,u5l->i - 1);
}

void u5l_i_forward(struct uint_5tuple_list *u5l){
	if(u5l->i + 1 >= u5l->len){
		// Cannot go beyond the length of list
		return;
	}
	
	++(u5l->i);
}

void u5l_i_backward(struct uint_5tuple_list *u5l){
	if(u5l->i == 0){
		// Cannot go beyond the beginning of the list
		return;
	}
	
	--(u5l->i);
}

void u5l_get(struct uint_5tuple_list *u5l,unsigned int (*val)[5]){
	if(u5l->i >= u5l->len){
		// Cannot read a value from an element that is not a part of the list
		return;
	}
	
	(*val)[0] = u5l->block[5 * u5l->i + 0];
	(*val)[1] = u5l->block[5 * u5l->i + 1];
	(*val)[2] = u5l->block[5 * u5l->i + 2];
	(*val)[3] = u5l->block[5 * u5l->i + 3];
	(*val)[4] = u5l->block[5 * u5l->i + 4];
}

void u5l_set(struct uint_5tuple_list *u5l,unsigned int val[5]){
	if(u5l->i >= u5l->len){
		// Cannot set a value to an element that is not a part of the list
		return;
	}
	
	u5l->block[5 * u5l->i + 0] = val[0];
	u5l->block[5 * u5l->i + 1] = val[1];
	u5l->block[5 * u5l->i + 2] = val[2];
	u5l->block[5 * u5l->i + 3] = val[3];
	u5l->block[5 * u5l->i + 4] = val[4];
}

void u5l_forall(struct uint_5tuple_list *u5l,void (*f)(unsigned int [5],unsigned int,unsigned int)){ // f(val,index,is_current)
	for(unsigned int i = 0;i < u5l->len;++i){
		(*f)(u5l->data + 5 * i,i,u5l->i == i);
	}
}