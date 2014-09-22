#include "math.h"

//Hacky implementations of math functions that don't exist in the mspgcc lib

int pow(int x, unsigned n) {

	int p,y;

	y = 1;
	p = x;
	while(1){
		if (n & 1) y = p*y;
		n = n >> 1;
		if (n == 0) return y;
		p = p*p;
	}
}


int log2(unsigned v){
	const unsigned int b[] = {0x2, 0xC, 0xF0, 0xFF00, 0xFFFF0000};
	const unsigned int S[] = {1, 2, 4, 8, 16};
	int i;

	unsigned int r = 0;
	for (i = 4; i >= 0; i--){
		if (v & b[i]){
			v >>= S[i];
			r |= S[i];
		}
	}	
	return r;
}

int floor(float x){
	return (int)(x + 0.5);
}
