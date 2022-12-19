#include <string.h>

#include "list.h"

#define LIST_DEFN(T,N,NAME,PREFIX)\
	\
	uint PREFIX ## _len(struct NAME *l){\
		return l->len;\
	}\
	\
	uint PREFIX ## _i  (struct NAME *l){\
		return l->i;\
	}\
	\
	void PREFIX ## _add_after(struct NAME *l,T val[N]){\
		if(l->len >= LIST_BLOCK_LEN){\
			/* List cannot exceed block size */\
			return;\
		}\
		\
		/* Insert at 0 if len == 0, otherwise insert at i + 1 */\
		uint insert_i = l->i + (l->len != 0);\
		\
		/* Move elements at and after the insertion position */\
		memmove(\
			l->block + N * (insert_i + 1),\
			l->block + N * (insert_i + 0),\
			(l->len - insert_i) * N * sizeof(T)\
		);\
		\
		++(l->len);\
		\
		/* Insert */\
		for(uint j = 0;j < N;++j){\
			l->block[N * insert_i + j] = val[j];\
		}\
	}\
	\
	void PREFIX ## _remove   (struct NAME *l){         \
		if(l->i >= l->len){\
			/* There is no valid element to remove at the length of the list */\
			/* This should handle attempts to remove from empty lists, since then i == len == 0 */\
			return;\
		}\
		\
		/* Move elements to overwrite current index */\
		memmove(\
			l->block + N * (l->i + 0),\
			l->block + N * (l->i + 1),\
			(l->len - l->i - 1) * N * sizeof(T)\
		);\
		\
		--(l->len);\
	}\
	\
	static void PREFIX ## _swap(struct NAME *l,uint i,uint j){\
		T temp[N];\
		\
		memcpy(temp            ,l->block + N * i,N * sizeof(T));\
		memcpy(l->block + N * i,l->block + N * j,N * sizeof(T));\
		memcpy(l->block + N * j,temp            ,N * sizeof(T));\
	}\
	\
	void PREFIX ## _val_forward (struct NAME *l){\
		if(l->i + 1 >= l->len){\
			/* Cannot move last element any more forward */\
			return;\
		}\
		\
		PREFIX ## _swap(l,l->i,l->i + 1);\
	}\
	\
	void PREFIX ## _val_backward(struct NAME *l){\
		if(l->i == 0){\
			/* Cannot move first element any more backward */\
			return;\
		}\
		\
		PREFIX ## _swap(l,l->i,l->i - 1);\
	}\
	\
	void PREFIX ## _i_forward (struct NAME *l){\
		if(l->i + 1 >= l->len){\
			/* Cannot go beyond the length of list */\
			return;\
		}\
		\
		++(l->i);\
	}\
	\
	void PREFIX ## _i_backward(struct NAME *l){\
		if(l->i == 0){\
			/* Cannot go beyond the beginning of the list */\
			return;\
		}\
		\
		--(l->i);\
	}\
	\
	void PREFIX ## _get(struct NAME *l,T val[N]){\
		if(l->i >= l->len){\
			/* Cannot read a value from an element that is not a part of the list */\
			return;\
		}\
		\
		for(uint j = 0;j < N;++j){\
			val[j] = l->block[N * l->i + j];\
		}\
	}\
	\
	void PREFIX ## _set(struct NAME *l,T val[N]){\
		if(l->i >= l->len){\
			/* Cannot set a value to an element that is not a part of the list */\
			return;\
		}\
		\
		for(uint j = 0;j < N;++j){\
			l->block[N * l->i + j] = val[j];\
		}\
	}\
	\
	void PREFIX ## _forall  (struct NAME *l,void (*f)(T [N],uint)){\
		for(uint i = 0;i < l->len;++i){\
			(*f)(l->block + N * i,i);\
		}\
	}\
	\
	void PREFIX ## _removeif(struct NAME *l,uint (*f)(T [N])){     \
		/* O(n) multiple-item remove: perform a copy of the whole list while skipping elements that */\
		/* are to be removed */\
		uint dest_i = 0;\
		\
		for(uint i = 0;i < l->len;++i){\
			if((*f)(l->block + N * i) != 0){\
				/* Element to be removed: skip it */\
				continue;\
			}\
			\
			/* Otherwise, element to remain in list: copy it */\
			if(dest_i != i){\
				/* Only copy if it is to change positions, however */\
				memcpy(l->block + N * dest_i,l->block + N * i,N * sizeof(T));\
			}\
			\
			++dest_i;\
		}\
		\
		/* Update length */\
		l->len = dest_i;\
	}\
	\
	uint PREFIX ## _contains(struct NAME *l,T val[N]){             \
		uint found = 0;\
		\
		for(uint i = 0;i < l->len;++i){\
			found = found || (memcmp(l->block + N * i,val,N * sizeof(T)) == 0);\
		}\
		\
		return found;\
	}

// uint-tuple lists ---------------------||
LIST_DEFN(uint,1,list_uint(1),lu(1))
LIST_DEFN(uint,3,list_uint(3),lu(3))
LIST_DEFN(uint,5,list_uint(5),lu(5))