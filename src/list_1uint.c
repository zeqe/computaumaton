#include <string.h>

#include "list_1uint.h"

unsigned int l1_len(struct list_1uint *l1){
	return l1->len;
}

unsigned int l1_i(struct list_1uint *l1){
	return l1->i;
}

void l1_init(struct list_1uint *l1){
	l1->len = 0;
	l1->i = 0;
}

void l1_add_after(struct list_1uint *l1,unsigned int val){
	if(l1->len >= BLOCK_1UINT_LEN){
		// List cannot exceed block size
		return;
	}
	
	// Insert at 0 if len = 0, otherwise insert at i + 1
	unsigned int insert_i = l1->i + (l1->len != 0);
	
	// Move elements at and after the insertion position
	memmove(
		l1->block + insert_i + 1,
		l1->block + insert_i + 0,
		(l1->len - insert_i) * sizeof(unsigned int)
	);
	
	++(l1->len);
	
	// Insert
	l1->block[insert_i] = val;
}

void l1_remove(struct list_1uint *l1){
	if(l1->i >= l1->len){
		// There is no valid element to remove at the length of the list
		// This shol1d handle attempts to remove from empty lists, since then len == i == 0
		return;
	}
	
	// Move elements to overwrite current index
	memmove(
		l1->block + l1->i + 0,
		l1->block + l1->i + 1,
		(l1->len - l1->i - 1) * sizeof(unsigned int)
	);
	
	--(l1->len);
}

static void l1_swap(struct list_1uint *l1,unsigned int i,unsigned int j){
	unsigned int temp;
	
	temp         = l1->block[i];
	l1->block[i] = l1->block[j];
	l1->block[j] = temp;
}

void l1_val_forward(struct list_1uint *l1){
	if(l1->i + 1 >= l1->len){
		// Cannot move last element any more forward
		return;
	}
	
	l1_swap(l1,l1->i,l1->i + 1);
}

void l1_val_backward(struct list_1uint *l1){
	if(l1->i == 0){
		// Cannot move first element any more backward
		return;
	}
	
	l1_swap(l1,l1->i,l1->i - 1);
}

void l1_i_forward(struct list_1uint *l1){
	if(l1->i + 1 >= l1->len){
		// Cannot go beyond the length of list
		return;
	}
	
	++(l1->i);
}

void l1_i_backward(struct list_1uint *l1){
	if(l1->i == 0){
		// Cannot go beyond the beginning of the list
		return;
	}
	
	--(l1->i);
}

void l1_get(struct list_1uint *l1,unsigned int *val){
	if(l1->i >= l1->len){
		// Cannot read a value from an element that is not a part of the list
		return;
	}
	
	(*val) = l1->block[l1->i];
}

void l1_set(struct list_1uint *l1,unsigned int val){
	if(l1->i >= l1->len){
		// Cannot set a value to an element that is not a part of the list
		return;
	}
	
	l1->block[l1->i] = val;
}

void l1_forall(struct list_1uint *l1,void (*f)(unsigned int,unsigned int)){ // f(val,index)
	for(unsigned int i = 0;i < l1->len;++i){
		(*f)(l1->block[i],i);
	}
}

void l1_removeif(struct list_1uint *l1,unsigned int (*f)(unsigned int)){ // f(val) returns to_be_deleted (0 = false, else true)
	// O(n) ml1tiple-item remove: perform a copy of the whole list while skipping elements that are to be removed
	unsigned int dest_i = 0;
	
	for(unsigned int i = 0;i < l1->len;++i){
		if((*f)(l1->block[i])){
			// Element to be removed: skip it
			continue;
		}
		
		// Otherwise, element to remain in list: copy it
		l1->block[dest_i] = l1->block[i];
		++dest_i;
	}
	
	// Update length
	l1->len = dest_i;
}

unsigned int l1_contains(struct list_1uint *l1,unsigned int val){
	unsigned int found = 0;
	
	for(unsigned int i = 0;i < l1->len;++i){
		found = found || (l1->block[i] == val);
	}
	
	return found;
}