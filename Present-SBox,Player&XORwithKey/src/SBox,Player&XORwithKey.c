/*
 ============================================================================
 Name        : SBox,Player&XORwithKey.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
unsigned long long int sblock(unsigned long long int input,unsigned long long int *s, int bits) {
	unsigned long long int i, z = 0;
	if (bits == 8) {   //lower 8 bit
		for (i = 0; i <= 4; i += 4) {
			z |= s[(((input & 0x00000000000000ff) >> (4 - i)) & 0xf)]
					<< (4 - i);
		}
	} else if (bits == 32) {     //for upper 32bits from 64 bit input
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

unsigned long long int pblock(unsigned long long int input,unsigned long long int *b, int bits) {
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
   /*      64 bit         128 bit
    *        |             |
             +-------------|
			 |             |
         ----------     ----------------
        |   S_BOX  |    |    Key       |
		|	P_BOX  |    |              |
		 ----------	    |   Scheduling |
		                ----------------
             |             |
			 |             |
			 +-------------|
			 |                                                      */

int main(void) {
	unsigned long long int k[] = { 0x1111222233334444, 0x1111111155555555 };
	unsigned long long int b[] = { 0, 16, 32, 48, 1, 17, 33, 49, 2, 18, 34, 50,
			3, 19, 35, 51, 4, 20, 36, 52, 5, 21, 37, 53, 6, 22, 38, 54, 7, 23,
			39, 55, 8, 24, 40, 56, 9, 25, 41, 57, 10, 26, 42, 58, 11, 27, 43,
			59, 12, 28, 44, 60, 13, 29, 45, 61, 14, 30, 46, 62, 15, 31, 47, 63 };
	unsigned long long int s[] = { 0x6, 0x3, 0xa, 0x5, 0xc, 0x8, 0x1, 0xb, 0x0,
			0xd, 0x9, 0xe, 0xf, 0x2, 0x7, 0x4 };
	unsigned long long input = 0x1234567890abcdef;
	unsigned long long int temp = 0, sblockop = 0, s1 = 0, p1 = 0;
	unsigned int  mainround = 0;
	for (mainround = 0; mainround < 32; mainround++) {
		input = input ^ k[1];       //xor input with lsb of key
		s1 = sblock(input, s, 64);  //s-box
		p1 = pblock(s1, b, 64);     //p-box(input of sbox)

        //Key Scheduling Algorithm
		temp = k[1];
		k[1] = (temp << 13) | (k[0] >> (64 - 13));
		k[0] = (temp >> (64 - 13)) | (k[0] << 13);
		sblockop = sblock(k[1], s, 8);

		k[1] = (k[1] & 0xffffffffffffff00) | (sblockop & 0xff);

		temp = (((k[1] & 0xf800000000000000) >> 59) ^ (mainround & 0b11111))<< 59;
		k[1] = (k[1] & 0x07ffffffffffffff) | temp;

		//  printf("Round no:%d,Key:%llx,%llx\n",round,k[0],k[1]);

		input = p1;
		printf("Cipher Text:%llx\n",p1);
	}

}
