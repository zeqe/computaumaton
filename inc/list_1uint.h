#ifndef LIST_1UINT_INCLUDED
	#define BLOCK_1UINT_LEN 256
	
	struct list_1uint{
		
		//           i=1  len=4
		//           |    |
		// block: [#,#,#,#,_,_,...]
		
		unsigned int block[BLOCK_1UINT_LEN];
		
		unsigned int len;
		unsigned int i;
		
		// block is the allocated space for the list to store its elements, which are unsigned integers
		// len is how many unsigned integers are stored in the block so far: len <= BLOCK_1UINT_LEN
		// i is the current unsigned integer's index: 0 <= i < len (unless len = 0, then i = len = 0)
	};
	
	unsigned int l1_len(struct list_1uint *l1);
	unsigned int l1_i(struct list_1uint *l1);
	
	void l1_init(struct list_1uint *l1);
	
	void l1_add_after(struct list_1uint *l1,unsigned int val);
	void l1_remove(struct list_1uint *l1);
	
	void l1_val_forward(struct list_1uint *l1);
	void l1_val_backward(struct list_1uint *l1);
	
	void l1_i_forward(struct list_1uint *l1);
	void l1_i_backward(struct list_1uint *l1);
	
	void l1_get(struct list_1uint *l1,unsigned int *val);
	void l1_set(struct list_1uint *l1,unsigned int val);
	
	void l1_forall(struct list_1uint *l1,void (*f)(unsigned int,unsigned int)); // f(val,index)
	void l1_removeif(struct list_1uint *l1,unsigned int (*f)(unsigned int)); // f(val) returns to_be_deleted (0 = false, else true)
	
	unsigned int l1_contains(struct list_1uint *l1,unsigned int val);
	
	#define LIST_1UINT_INCLUDED
#endif