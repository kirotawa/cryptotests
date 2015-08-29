#ifndef __CRYPTOTEST
#define __CRYPTOTEST

#define AES 	0x0
#define CBC 	0x1
#define CTR 	0x2
#define ECB 	0x3
#define GCM 	0x4
#define GHASH 	0x5
#define SHA256  0x6
#define SHA512	0x7

int sha_test(int, char*);

#endif //__CRYPTOTEST
