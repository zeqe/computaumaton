#ifndef UINT_LIST_INCLUDED
	#define UINT_BLOCK_LEN 256
	
	struct uint_list{
		
		//           i=1  len=4
		//           |    |
		// block: [#,#,#,#,_,_,...]
		
		unsigned int block[UINT_BLOCK_LEN];
		
		unsigned int len;
		unsigned int i;
		
		// block is the allocated space for the list to store its elements, which are unsigned integers
		// len is how many unsigned integers are stored in the block so far: len <= UINT_BLOCK_LEN
		// i is the current unsigned integer's index: 0 <= i < len (unless len = 0, then i = len = 0)
	};
	
	unsigned int ul_len(struct uint_list *ul);
	unsigned int ul_i(struct uint_list *ul);
	
	void ul_init(struct uint_list *ul);
	
	void ul_add_after(struct uint_list *ul,unsigned int val);
	void ul_remove(struct uint_list *ul);
	
	void ul_val_forward(struct uint_list *ul);
	void ul_val_backward(struct uint_list *ul);
	
	void ul_i_forward(struct uint_list *ul);
	void ul_i_backward(struct uint_list *ul);
	
	void ul_get(struct uint_list *ul,unsigned int *val);
	void ul_set(struct uint_list *ul,unsigned int val);
	
	void ul_forall(struct uint_list *ul,void (*f)(unsigned int,unsigned int)); // f(val,index)
	void ul_removeif(struct uint_list *ul,unsigned int (*f)(unsigned int)); // f(val) returns to_be_deleted (0 = false, else true)
	
	unsigned int ul_contains(struct uint_list *ul,unsigned int val);
	
	#define UINT_LIST_INCLUDED
#endif