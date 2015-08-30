#ifndef __CRYPTOTEST
#define __CRYPTOTEST

#define AES 	0x00
#define CBC 	0x01
#define CTR 	0x02
#define ECB 	0x03
#define GCM 	0x04
#define GHASH 	0x05
#define SHA256  0x06
#define SHA512	0x07

#define hexdump(value) { \
	print_hex_dump(KERN_CONT, "", DUMP_PREFIX_OFFSET, \
			16, 1,\
			value, sizeof(value), false); \
	}

int sha_test(int, char*);
int ghash_test(char*, int, char*);

#endif //__CRYPTOTEST
