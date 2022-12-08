#ifndef UINT_5TUPLE_LIST_INCLUDED
	#define UINT_5TUPLE_BLOCK_LEN 256
	
	struct uint_5tuple_list{
		
		//                     [---i=1---]            |len=3
		//                     \         /            |
		// block: [(#,#,#,#,#),(#,#,#,#,#),(#,#,#,#,#),(_,_,_,_,_),(_,_,_,_,_),...]
		
		unsigned int block[5 * UINT_5TUPLE_BLOCK_LEN];
		
		unsigned int len;
		unsigned int i;
		
		// block is the allocated space for the list to store its elements, which are 5-tuples of unsigned integers
		// len is how many 5-tuples are stored in the block so far: len <= UINT_5TUPLE_BLOCK_LEN
		// i is the current 5-tuple's index: 0 <= i < len (unless len = 0, then i = len = 0)
	};
	
	void u5l_init(struct uint_5tuple_list *u5l);
	unsigned int u5l_len(struct uint_5tuple_list *u5l);
	
	void u5l_add_after(struct uint_5tuple_list *u5l,unsigned int val[5]);
	void u5l_remove(struct uint_5tuple_list *u5l);
	
	void u5l_val_forward(struct uint_5tuple_list *u5l);
	void u5l_val_backward(struct uint_5tuple_list *u5l);
	
	void u5l_i_forward(struct uint_5tuple_list *u5l);
	void u5l_i_backward(struct uint_5tuple_list *u5l);
	
	void u5l_get(struct uint_5tuple_list *u5l,unsigned int (*val)[5]);
	void u5l_set(struct uint_5tuple_list *u5l,unsigned int val[5]);
	
	void u5l_forall(struct uint_5tuple_list *u5l,void (*f)(unsigned int [5],unsigned int,unsigned int)); // f(val,index,is_current)
	
	#define UINT_5TUPLE_LIST_INCLUDED
#endif