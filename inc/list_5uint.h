#ifndef LIST_5UINT_INCLUDED
	#define BLOCK_5UINT_LEN 256
	
	struct list_5uint{
		
		//                     [---i=1---]            |len=3
		//                     \         /            |
		// block: [(#,#,#,#,#),(#,#,#,#,#),(#,#,#,#,#),(_,_,_,_,_),(_,_,_,_,_),...]
		
		unsigned int block[5 * BLOCK_5UINT_LEN];
		
		unsigned int len;
		unsigned int i;
		
		// block is the allocated space for the list to store its elements, which are 5-tuples of unsigned integers
		// len is how many 5-tuples are stored in the block so far: len <= BLOCK_5UINT_LEN
		// i is the current 5-tuple's index: 0 <= i < len (unless len = 0, then i = len = 0)
	};
	
	unsigned int l5_len(struct list_5uint *l5);
	unsigned int l5_i(struct list_5uint *l5);
	
	void l5_init(struct list_5uint *l5);
	
	void l5_add_after(struct list_5uint *l5,unsigned int val[5]);
	void l5_remove(struct list_5uint *l5);
	
	void l5_val_forward(struct list_5uint *l5);
	void l5_val_backward(struct list_5uint *l5);
	
	void l5_i_forward(struct list_5uint *l5);
	void l5_i_backward(struct list_5uint *l5);
	
	void l5_get(struct list_5uint *l5,unsigned int (*val)[5]);
	void l5_set(struct list_5uint *l5,unsigned int val[5]);
	
	void l5_forall(struct list_5uint *l5,void (*f)(unsigned int [5],unsigned int)); // f(val,index)
	void l5_removeif(struct list_5uint *l5,unsigned int (*f)(unsigned int [5])); // f(val) returns to_be_deleted (0 = false, else true)
	
	unsigned int l5_contains(struct list_5uint *l5,unsigned int val[5]);
	
	#define LIST_5UINT_INCLUDED
#endif