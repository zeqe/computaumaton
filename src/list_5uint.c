#include <string.h>

#include "list_5uint.h"

unsigned int l5_len(struct list_5uint *l5){
	return l5->len;
}

unsigned int l5_i(struct list_5uint *l5){
	return l5->i;
}

void l5_init(struct list_5uint *l5){
	l5->len = 0;
	l5->i = 0;
}

void l5_add_after(struct list_5uint *l5,unsigned int val[5]){
	if(l5->len >= BLOCK_5UINT_LEN){
		// List cannot exceed block size
		return;
	}
	
	// Insert at 0 if len = 0, otherwise insert at i + 1
	unsigned int insert_i = l5->i + (l5->len != 0);
	
	// Move elements at and after the insertion position
	memmove(
		l5->block + 5 * (insert_i + 1),
		l5->block + 5 * (insert_i + 0),
		5 * (l5->len - insert_i) * sizeof(unsigned int)
	);
	
	++(l5->len);
	
	// Insert
	l5->block[5 * insert_i + 0] = val[0];
	l5->block[5 * insert_i + 1] = val[1];
	l5->block[5 * insert_i + 2] = val[2];
	l5->block[5 * insert_i + 3] = val[3];
	l5->block[5 * insert_i + 4] = val[4];
}

void l5_remove(struct list_5uint *l5){
	if(l5->i >= l5->len){
		// There is no valid element to remove at the length of the list
		// This should handle attempts to remove from empty lists, since then len == i == 0
		return;
	}
	
	// Move elements to overwrite current index
	memmove(
		l5->block + 5 * (l5->i + 0),
		l5->block + 5 * (l5->i + 1),
		5 * (l5->len - l5->i - 1) * sizeof(unsigned int)
	);
	
	--(l5->len);
}

static void l5_swap(struct list_5uint *l5,unsigned int i,unsigned int j){
	unsigned int temp[5];
	
	memcpy(temp              ,l5->block + 5 * i,5 * sizeof(unsigned int));
	memcpy(l5->block + 5 * i,l5->block + 5 * j,5 * sizeof(unsigned int));
	memcpy(l5->block + 5 * j,temp              ,5 * sizeof(unsigned int));
}

void l5_val_forward(struct list_5uint *l5){
	if(l5->i + 1 >= l5->len){
		// Cannot move last element any more forward
		return;
	}
	
	l5_swap(l5,l5->i,l5->i + 1);
}

void l5_val_backward(struct list_5uint *l5){
	if(l5->i == 0){
		// Cannot move first element any more backward
		return;
	}
	
	l5_swap(l5,l5->i,l5->i - 1);
}

void l5_i_forward(struct list_5uint *l5){
	if(l5->i + 1 >= l5->len){
		// Cannot go beyond the length of list
		return;
	}
	
	++(l5->i);
}

void l5_i_backward(struct list_5uint *l5){
	if(l5->i == 0){
		// Cannot go beyond the beginning of the list
		return;
	}
	
	--(l5->i);
}

void l5_get(struct list_5uint *l5,unsigned int (*val)[5]){
	if(l5->i >= l5->len){
		// Cannot read a value from an element that is not a part of the list
		return;
	}
	
	(*val)[0] = l5->block[5 * l5->i + 0];
	(*val)[1] = l5->block[5 * l5->i + 1];
	(*val)[2] = l5->block[5 * l5->i + 2];
	(*val)[3] = l5->block[5 * l5->i + 3];
	(*val)[4] = l5->block[5 * l5->i + 4];
}

void l5_set(struct list_5uint *l5,unsigned int val[5]){
	if(l5->i >= l5->len){
		// Cannot set a value to an element that is not a part of the list
		return;
	}
	
	l5->block[5 * l5->i + 0] = val[0];
	l5->block[5 * l5->i + 1] = val[1];
	l5->block[5 * l5->i + 2] = val[2];
	l5->block[5 * l5->i + 3] = val[3];
	l5->block[5 * l5->i + 4] = val[4];
}

void l5_forall(struct list_5uint *l5,void (*f)(unsigned int [5],unsigned int)){ // f(val,index)
	for(unsigned int i = 0;i < l5->len;++i){
		(*f)(l5->block + 5 * i,i);
	}
}

void l5_removeif(struct list_5uint *l5,unsigned int (*f)(unsigned int [5])){ // f(val) returns to_be_deleted (0 = false, else true)
	// O(n) multiple-item remove: perform a copy of the whole list while skipping elements that are to be removed
	unsigned int dest_i = 0;
	
	for(unsigned int i = 0;i < l5->len;++i){
		if((*f)(l5->block + 5 * i)){
			// Element to be removed: skip it
			continue;
		}
		
		// Otherwise, element to remain in list: copy it
		if(dest_i != i){
			// Only copy if it is to change positions, however
			memcpy(l5->block + 5 * dest_i,l5->block + 5 * i,5 * sizeof(unsigned int));
		}
		
		++dest_i;
	}
	
	// Update length
	l5->len = dest_i;
}

unsigned int l5_contains(struct list_5uint *l5,unsigned int val[5]){
	unsigned int found = 0;
	
	for(unsigned int i = 0;i < l5->len;++i){
		found = found || (memcmp(l5->block + 5 * i,val,5 * sizeof(unsigned int)) == 0);
	}
	
	return found;
}