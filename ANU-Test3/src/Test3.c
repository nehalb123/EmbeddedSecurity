/*
 ============================================================================
 Name        : Test3.c
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
	if (bits == 8) {
		for (i = 0; i <= 4; i += 4) {
			z |= s[(((input & 0x00000000000000ff) >> (4 - i)) & 0xf)]<< (4 - i);
		}
	} else if (bits == 32) {
		for (i = 0; i < 32; i += 4) {
			z |= s[((input >> (28 - i)) & 0xf)]<<(28 - i);
		}
	} else if (bits == 64) {
		for (i = 0; i < 64; i += 4) {
			z |= s[((input >> (60 - i)) & 0xf)]<<(60 - i);
		}
	}
	return z;
}

unsigned long long int pblock(unsigned long long int input,unsigned long long int *b, int bits) {
	unsigned long long int d = 0, i;
	if (bits == 32) {
		for (i = 0; i < 32; i++) {
			d |= (((input) >> i) & 0x1) << b[i];
		}
	} else if (bits == 64) {

		for (i = 0; i < 64; i++) {
			d |= ((input >> i) & 0x1) << b[i];
		}
	}

	return d;
}

int main(void) {
	unsigned long long int k[] = { 0x1111222233334444, 0x1111222233334444 };
    unsigned long long int p[]={20,16,28,24,17,21,25,29,22,18,30,26,19,23,27,31,11,15,3,7,14,10,6,2,9,13,1,5,12,8,4,0};
    unsigned long long int s[]={0x2,0x9,0x7,0xe,0x1,0xc,0xa,0x0,0x4,0x3,0x8,0xd,0xf,0x6,0x5,0xb};
    unsigned long long int c=0x1234567890abcdef,z=0,round=0,sblockop=0,temp=0;
    unsigned long long int input1=((c&0xffffffff00000000)>>32),input2=(c&0x00000000ffffffff),y1=0;
    unsigned int y=0,sblockop1=0,sblockop2=0,z1=0;

      for(round=0;round<25;round++){
       z=(input1<<3)|((input1>>(32-3)));
       sblockop1=sblock(z,s,32);
       y =(sblockop1^(input2)) ;
       z1=(((input1)>>8)|((input1)<<(32-8)))&0xffffffff;
       sblockop2=sblock(z1,s,32)&0xffffffff;
       y1=(y^sblockop2)^((k[1])&0x00000000ffffffff);

        temp = k[1];
   		k[1] = (temp << 13) | (k[0] >> (64 - 13));
   		k[0] = (temp >> (64 - 13)) | (k[0] << 13);
   		sblockop = sblock(k[1], s, 8);

   		k[1] = (k[1] & 0xffffffffffffff00) | (sblockop & 0xff);

   		temp = (((k[1] & 0xf800000000000000) >> 59) ^ (round & 0b11111))<< 59;
   		k[1] = (k[1] & 0x07ffffffffffffff) | temp;

   		input2=pblock(input1,p,32);
   		input1=pblock(y1,p,32);

   		printf("Output:%llx%llx\n",input1,input2);
  }
}
