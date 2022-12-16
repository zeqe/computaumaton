#ifndef SYMBOL_INCLUDED
	
	/*
	symbol: a series of bytes packed into a single unsigned integral value
	- char:      >= 8  bits, 1 byte
	- short:     >= 16 bits, 2 bytes
	- int:       >= 16 bits, 2 bytes
	- long:      >= 32 bits, 4 bytes
	- long long: >= 64 bits, 8 bytes
	*/
	
	#define SYMBOL_SET(symbol,index,byte) (((symbol) & ~(0xff << (8 * (index)))) | (((byte) & 0xff) << (8 * (index))))
	#define SYMBOL_GET(symbol,index) (((symbol) & (0xff << (8 * (index)))) >> (8 * (index)))
	
	#define SYMBOL_INCLUDED
#endif