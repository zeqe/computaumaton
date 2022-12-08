#ifndef UINT_BLOCK_LIST_INCLUDED
	#define UINT_BLOCK_SIZE 256
	
	struct uint_block_list{
		unsigned int block[UINT_BLOCK_SIZE];
		
		unsigned int len;
		unsigned int i;
		
		// block is the allocated space for the list to store its information;
		// len is how much of the block has been used so far
		// i is the current modification position: 0 <= i <= len
		
		//          i      len
		//          |      |
		// [--------*------:           block]
	};
	
	void ubl_init(struct uint_block_list *ubl);
	
	void ubl_add(struct uint_block_list *ubl,unsigned int val);
	void ubl_remove(struct uint_block_list *ubl);
	
	void ubl_forward(struct uint_block_list *ubl);
	void ubl_backward(struct uint_block_list *ubl);
	
	unsigned int ubl_get(struct uint_block_list *ubl);
	void ubl_set(struct uint_block_list *ubl,unsigned int val);
	
	// Note that in the spirit of emulating mathematical sets, ubl_add() and ubl_set()
	// will not allow duplicate values to exist in the list.
	
	#define UINT_BLOCK_LIST_INCLUDED
#endif