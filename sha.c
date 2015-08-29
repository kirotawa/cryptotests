#include <crypto/sha.h>
#include <crypto/hash.h>

#include "cryptotest.h"

#define HUNK(type) (0x80 >> (1 - type)) 
#define DIGEST(type) (0x20 << type)


int sha_test(int type, char *alg)
{
	struct crypto_shash *tfm;
	u8 msg[HUNK(type) + 1];
	u8 hash[DIGEST(type)];
	int rc = 0;
	int i;

	memset(msg, 0xab, HUNK(type) + 1);
	memset(hash, 0x00, DIGEST(type));
	
	tfm = crypto_alloc_shash(alg, 0, 0);
	if (IS_ERR(tfm)) {
		rc = PTR_ERR(tfm);
		printk("Failed to allocate transformation: %d\n", rc);
	}

	{
		struct {
			struct shash_desc desc;
			char ctx[crypto_shash_descsize(tfm)];	
		} desc;
		
		desc.desc.tfm = tfm;
		desc.desc.flags = 0;
		
		crypto_shash_init(&desc.desc);
		crypto_shash_update(&desc.desc, msg, HUNK(type) + 1);
		crypto_shash_final(&desc.desc, hash);
		
	}	 
	
	printk("%s: ", alg);
	for (i = 0; i < sizeof(hash); i++)
		printk(" %02x", (u8) hash[i]);

	printk("\n");
	
	crypto_free_shash(tfm);
	return 0;
}
