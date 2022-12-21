#include "uint.h"
#include "set.h"
#include "automaton.h"

enum fsa_edit_state{
	FSA_EDIT_S,
	FSA_EDIT_Q,
	FSA_EDIT_Q0,
	FSA_EDIT_D,
	FSA_EDIT_F,
	FSA_EDIT_I,
	
	FSA_EDIT_COUNT
};

struct finite_state_automaton{
	
	// Program state
	
	enum automaton_state state;
	enum fsa_edit_state edit_state;
	
	// M = (S,Q,q0,D,F)
	// 
	// - S  (sigma)             : input alphabet
	// - Q                      : states
	// - q0 elemnt of Q         : initial state
	// - D  subset of Q x S x Q : transition relation
	// - F  subset of S         : final states
	
	struct set(1) S,      // list of chars
	struct set(1) Q,      // list of two-letter symbols
	struct element(1) q0, // two-letter symbol, element of Q
	struct set(3) D,      // list of (element of Q,element of S,element of Q,0,0)
	struct set(1) F,      // list of chars, elements of S
	
	// C = (q,I)
	// 
	// - q elemnt of Q   : current state
	// - I subset of S^n : input string of length n
	
	uint q;          // two-letter symbol, element of Q
	struct set(1) I; // list of chars, elements of S
};















#define M_SET_1       (NAME,  MB)\
	1,SET    ,NAME,MB ,NULL
#define M_SUBSET_OF_1 (NAME,  W1,T1,N1,MB1,S1)\
	1,SET    ,NAME,MB1,N1
#define M_SUBSET_OF_3 (NAME,  W1,T1,N1,MB1,S1,  W2,T2,N2,MB2,S2,  W3,T3,N3,MB3,S3)\
	3,SET    ,NAME,MB1,MB2,MB3,N1,N2,N3
#define M_SUBSET_OF_5 (NAME,  W1,T1,N1,MB1,S1,  W2,T2,N2,MB2,S2,  W3,T3,N3,MB3,S3,  W4,T4,N4,MB4,S4,  W5,T5,N5,MB5,S5)\
	5,SET    ,NAME,MB1,MB2,MB3,MB4,MB5,N1,N2,N3,N4,N5
#define M_ELEMENT_OF_1(NAME,  W1,T1,N1,MB1,S1)\
	1,ELEMENT,NAME,MB1,N1

#define M_ELEMENT_T(N) element(N)
#define M_SET_T(N)     set(N)

#define M_DECL(WIDTH,TYPE,NAME,MAX_BYTES,SRCS) struct M_ ## TYPE ## _T(WIDTH) *NAME

#define M_INIT_1(MB1,S1)\
	{MB1},{S1}
#define M_INIT_3(MB1,MB2,MB3,S1,S2,S3)\
	{MB1,MB2,MB3},{S1,S2,S3}
#define M_INIT_5(MB1,MB2,MB3,MB4,MB5,S1,S2,S3,S4,S5)\
	{MB1,MB2,MB3,MB4,MB5},{S1,S2,S3,S4,S5}

#define M_INIT(WIDTH,TYPE,NAME,...) TYPE ## _INIT(M_INIT_ ## WIDTH (__VA_ARGS__))







#define COMMA ,

#define M_SET       (I,N,NAME,MAX_BYTES)      I,SET    ,N,NAME,MAX_BYTES,    ,
#define M_ELEMENT_OF(I,N,NAME,MAX_BYTES,SRCS) I,ELEMENT,N,NAME,MAX_BYTES,SRCS,
#define M_SUBSET_OF (I,N,NAME,MAX_BYTES,SRCS) I,SET    ,N,NAME,MAX_BYTES,SRCS,

#define ELEMENT_T(N) element(N)
#define SET_T(N) set(N)

#define LISTIFY1(A,S1)       &(A.S1.list)
#define LISTIFY3(A,S1,S2,S3) &(A.S1.list),&(A.S2.list),&(A.S3.list)

#define M_DECL(I,TYPE,N,NAME,MAX_BYTES,SRCS) struct TYPE ## _T(N) NAME,
#define M_INIT(I,TYPE,N,NAME,MAX_BYTES,SRCS) TYPE ## _INIT({MAX_BYTES},{LISTIFY ## N(I,SRCS)}),

#define FSA\
	M_SET1       (S ,1                                  )\
	M_SET1       (Q ,2                                  )\
	M_ELEMENT1_OF(q0,2                ,Q                )\
	M_SUBSET3_OF (D ,2 COMMA 1 COMMA 2,Q COMMA S COMMA Q)\
	M_SUBSET1_OF (F ,2                ,Q                )







#define M_DEPV(A,T,n)
#define M_DEP0(A,T,n)                 T ## _INIT({n},{NULL}),
#define M_DEP1(A,T,n,s)               T ## _INIT({n},{&(A.s.list)}),
#define M_DEP3(A,T,n1,n2,n3,s1,s2,s3) T ## _INIT({n1,n2,n3},{&(A.s1.list),&(A.s2.list),&(A.s3.list)}),

#define AUTOMATON_DEP(A,D1,T1,A1,D2,T2,A2,D3,T3,A3,D4,T4,A4,D5,T5,A5,D6,T6,A6,D7,T7,A7)\
	M_DEP ## D1 (A,T1,A1)\
	M_DEP ## D2 (A,T2,A2)\
	M_DEP ## D3 (A,T3,A3)\
	M_DEP ## D4 (A,T4,A4)\
	M_DEP ## D5 (A,T5,A5)\
	M_DEP ## D6 (A,T6,A6)\
	M_DEP ## D7 (A,T7,A7)

#define FSA_DEP(FSA) AUTOMATON_DEP(\
	0, SET    , 1                                        ,\
	0, SET    , 2                                        ,\
	1, ELEMENT, 2                 COMMA Q                ,\
	3, SET    , 2 COMMA 1 COMMA 2 COMMA Q COMMA S COMMA Q,\
	1, SET    , 2                 COMMA Q                ,\
	V,        ,                                          ,\
	V,        ,                                          ,\
)

#define FSA_DEP(FSA) AUTOMATON_DEP(\
	DEP0(FSA,SET    ,1          )\
	DEP0(FSA,SET    ,2          )\
	DEP1(FSA,ELEMENT,2          )\
	DEP3(FSA,SET    ,2,1,2,Q,S,Q)\
	DEP1(FSA,SET    ,1    ,Q    )\
	DEPV\
	DEPV\
)

#define M_M(N,)

#define FSA_DEP(FSA) AUTOMATON_DEP(\
	DEP0(FSA,SET    ,1          )\
	DEP0(FSA,SET    ,2          )\
	DEP1(FSA,ELEMENT,2          )\
	DEP3(FSA,SET    ,2,1,2,Q,S,Q)\
	DEP1(FSA,SET    ,1    ,Q    )\
	DEPV\
	DEPV\
)



#define FSA_INIT() AUTOMATON_INIT()

#define SET_INIT(D) SET_INIT(D)
#define EL_INIT(D) ELEMENT_INIT(D)

#define AUTOMATON_DECL
	struct NAME{
		struct element(E1N) E1NAME;
		struct element(E2N) E2NAME;
		
		struct set(S1N) S1NAME;
		struct set(S1N) S1NAME;
		struct set(S1N) S1NAME;
		struct set(S1N) S1NAME;
		struct set(S1N) S1NAME;
	};

#define AUTOMATON_INIT(D1,D2,D3,D4,D5,D6,D7)
#define AUTOMATON_INIT(E1,E2,S1,S2,S3,S4,S5)
	AUTOMATON_IDLE,
	0,
	
	E1 E2
	S1 S2 S3 S4 S5
	

#define FSA_INIT(FSA) {\
	AUTOMATON_IDLE,\
	FSA_EDIT_S,    \
	\
	SET_INIT    (d0(FSA,1          )),\
	SET_INIT    (d0(FSA,2          )),\
	ELEMENT_INIT(d1(FSA,2    ,Q    )),\
	SET_INIT    (d3(FSA,2,1,2,Q,S,Q)),\
	SET_INIT    (d1(FSA,1    ,S    )),\
	\
	ELEMENT_INIT(d1(FSA,2,Q)),\
	SET_INIT(d1(FSA,1,S))\
}

#define FSA_INIT(FSA) {\
	AUTOMATON_IDLE,\
	FSA_EDIT_S,    \
	\
	SET_INIT    ({1}    ,{NULL}                                     ),\
	SET_INIT    ({2}    ,{NULL}                                     ),\
	ELEMENT_INIT({2}    ,{&(FSA.S.list)}                            ),\
	SET_INIT    ({2,1,2},{&(FSA.Q.list),&(FSA.S.list),&(FSA.Q.list)}),\
	SET_INIT    ({1}    ,{&(FSA.S.list)}                            ),\
	\
	0,                       \
	SET_INIT({1},{&(FSA.S)})\
}

#define FSA_INIT(FSA) {\
	AUTOMATON_IDLE,\
	FSA_EDIT_S,    \
	\
	SET_INIT({1},{NULL}),\
	SET_INIT({2},{NULL}),\
	ELEMENT_INIT({2},{&(FSA.S.list)}),\
	SET_INIT({2,1,2},{&(FSA.Q.list),&(FSA.S.list),&(FSA.Q.list)}),\
	SET_INIT({1},{&(FSA.S.list)}),                      \
	\
	0,                       \
	SET_INIT({1},{&(FSA.S)})\
}

uint fsa_update(struct finite_state_automaton *fsa,int in){
	uint can_switch = 0;
	
	switch(fsa->state){
	case AUTOMATON_IDLE:
		switch(fsa->edit_state){
		case FSA_EDIT_S:
			can_switch = set(1) ## _update(&(fsa->S),in);
			
			break;
		case FSA_EDIT_Q:
			can_switch = set(1) ## _update(&(fsa->Q),in);
			
			break;
		case FSA_EDIT_Q0:
			can_switch = set(1) ## _update(in);
			
			break;
		case FSA_EDIT_D:
			can_switch = set(3) ## _update(&(fsa->D),in);
			
			break;
		case FSA_EDIT_F:
			can_switch = set(1) ## _update(&(fsa->F),in);
			
			break;
		case FSA_EDIT_I:
			can_switch = set(1) ## _update(&(fsa->I),in);
			
			break;
		}
		
		if(can_switch && (in == KEY_UP || in == KEY_DOWN)){
			fsa->edit_state = (enum fsa_edit_state)(
				((uint)FSA_EDIT_COUNT + (uint)fsa->edit_state + (in == KEY_DOWN) - (in == KEY_UP)) % (uint)FSA_EDIT_COUNT
			);
		}
		
		return can_switch;
		
	case AUTOMATON_STEPPING:
		
	case AUTOMATON_RUNNING:
		
	}
}