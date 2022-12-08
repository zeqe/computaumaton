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
		// i is the current unsigned integer's index: 0 <= i <= len
		
		// allowing i = len:
		// 
		//                i=len=4
		//                |
		// block: [#,#,#,#,_,_,...]
		// 
		// - allows insertion of an element to the end with add()
		// - must cause remove() to do nothing at that location, since there is no element to remove
	};
	
	void ul_init(struct uint_list *ul);
	unsigned int ul_len(struct uint_list *ul);
	
	void ul_add(struct uint_list *ul,unsigned int val);
	void ul_remove(struct uint_list *ul);
	
	void ul_val_forward(struct uint_list *ul);
	void ul_val_backward(struct uint_list *ul);
	
	void ul_i_forward(struct uint_list *ul);
	void ul_i_backward(struct uint_list *ul);
	
	void ul_get(struct uint_list *ul,unsigned int *val);
	void ul_set(struct uint_list *ul,unsigned int val);
	
	void ul_forall(struct uint_list *ul,void (*f)(unsigned int,unsigned int,unsigned int)); // f(val,index,is_current)
	
	#define UINT_LIST_INCLUDED
#endif