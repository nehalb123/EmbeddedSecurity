/*
 ============================================================================
 Name        : 8,4bitSBoxWith32bitPlayerWithXOR.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
unsigned long long int sblock(unsigned long long int input,
		unsigned long long int *s, int bits) {
	unsigned long long int i, z = 0;
	if (bits == 32) {     //for upper 32bits from 64 bit input
		for (i = 0; i < 32; i += 4) {
			z |= s[((((input & 0xffffffff00000000) >> 32) >> (28 - i)) & 0xf)]
					<< (28 - i);
		}
	} else if (bits == 64) {
		for (i = 0; i < 64; i += 4) {
			z |= s[((input >> (60 - i)) & 0xf)] << (60 - i);
		}
	}
	return z;

}

unsigned long long int pblock(unsigned long long int input,
		unsigned long long int *b, int bits) {
	unsigned long long int d = 0, i;
	if (bits == 32) {    //lower 32 bits of 64 bit input
		for (i = 0; i < 32; i++) {
			d |= (((input & 0x00000000ffffffff) >> i) & 0x1) << b[i];
		}
	} else if (bits == 64) {

		for (i = 0; i < 64; i++) {
			d |= ((input >> i) & 0x1) << b[i];
		}
	}

	return d;
}

int main(void) {
	unsigned long long int b[] = { 16, 17, 18, 19, 20, 21, 22, 23, 8, 9, 10, 11, 12, 13, //Array of Permutation
			14, 15, 0, 1, 2, 3, 4, 5, 6, 7, 24, 25, 26, 27, 28, 29, 30, 31 };
	unsigned long long int s[] = { 0x3, 0xf, 0xe, 0x1, 0x0, 0xa, 0x5, 0x8, 0xc,
			0x4, 0xb, 0x2, 0x9, 0x7, 0x6, 0xd };
	unsigned long long int c = 0x1234567890abcdef, i, sblockop = 0,
			pblockop = 0, x1, x2, z1 = 0, d1 = 0;

	sblockop = sblock(c, s, 32);
	/*for (i = 0; i < 32; i += 4) {
	 sblockop |= s[((((c&0xffffffff00000000)>>32) >> (28 - i)) & 0xf)] << (28 - i);
	 }*/


    pblockop=pblock(c,b,32);
	/*for (i = 0; i < 32; i++) {
		pblockop |= (((c & 0x00000000ffffffff) >> i) & 0x1) << b[i];
	}*/
	x1 = sblockop ^ pblockop;
	x2 = sblockop ^ x1;
	for (i = 0; i < 32; i += 4) {
		z1 |= s[((x1 >> (28 - i)) & 0xf)] << (28 - i);
	}
	for (i = 0; i < 32; i++) {
		d1 |= ((x2 >> i) & 0x1) << b[i];
	}
	printf("%llx%llx", d1, z1);

}
