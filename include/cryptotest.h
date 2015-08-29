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

#define hexdump(value) { \
	print_hex_dump(KERN_CONT, "", DUMP_PREFIX_OFFSET, \
			16, 1,\
			value, sizeof(value), false); \
	}

int sha_test(int, char*);

#endif //__CRYPTOTEST
