#include <string.h>

#include "set_objects.h"

// element ------------------------------||
#define ELEMENT_DEFN(N)\
	uint element(N) ## _update(struct element(N) *element,int in){\
		if(element->is_reading){\
			/* Continue reading new value */\
			uint complete = symread(N) ## _update(&(element->read),in);\
			\
			if(complete){\
				/* Store new value once read is complete */\
				memcpy(&(element->value),symread(N) ## _get(&(element->read)),N * sizeof(uint));\
				element->is_specified = 1;\
				\
				element->is_reading = 0;\
			}\
			\
		}else{\
			/* Perform atomic operations */\
			switch(in){\
			case '<':\
				element->is_specified = 0;\
				\
				break;\
			case ',':\
				symread(N) ## _init(&(element->read));\
				element->is_reading = 1;              \
				\
				break;\
			}\
		}\
		\
		return !(element->is_reading);\
	}

ELEMENT_DEFN(1)
ELEMENT_DEFN(3)
ELEMENT_DEFN(5)

// set ----------------------------------||
#define SET_DEFN(N)\
	uint set(N) ## _update(struct set(N) *set,int in){\
		if(set->is_reading){\
			/* Continue reading new value */\
			uint complete = symread(N) ## _update(&(set->read),in);\
			\
			if(complete){\
				/* Attempt to add new value to the set once read is complete */\
				if(!lu(N) ## _contains(&(set->list),symread(N) ## _get(&(set->read)))){\
					lu(N) ## _add_after(&(set->list),symread(N) ## _get(&(set->read)));\
				}\
				\
				set->is_reading = 0;\
			}\
			\
		}else{\
			/* Perform atomic operations */\
			switch(in){\
			case KEY_LEFT:\
				lu(N) ## _i_backward(&(set->list));\
				\
				break;\
			case KEY_RIGHT:\
				lu(N) ## _i_forward(&(set->list));\
				\
				break;\
			case '(':\
				lu(N) ## _val_backward(&(set->list));\
				lu(N) ## _i_backward(&(set->list));  \
				\
				break;\
			case ')':\
				lu(N) ## _val_forward(&(set->list));\
				lu(N) ## _i_forward(&(set->list));  \
				\
				break;\
			case '<':\
				lu(N) ## _remove(&(set->list));\
				\
				break;\
			case ',':\
				symread(N) ## _init(&(set->read));\
				set->is_reading = 1;              \
				\
				break;\
			}\
		}\
		\
		return !(set->is_reading);\
	}

SET_DEFN(1)
SET_DEFN(3)
SET_DEFN(5)