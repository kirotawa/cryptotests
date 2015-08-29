#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/types.h>
#include <linux/init.h>
#include <linux/err.h>

#include "cryptotest.h"

static int alg;

static int __init cryptotest_init(void)
{
	switch (alg) {
		case AES:
			printk(KERN_INFO "Running test for aes\n");
			break;
		case CBC:
			printk(KERN_INFO "Running test for cbc\n");
			break;
		case CTR:
			printk(KERN_INFO "Running test for ctr\n");
			break;
		case ECB:	
			printk(KERN_INFO "Running test for ecb\n");
			break;
		case GCM:
			printk(KERN_INFO "Running test for gcm\n");
			break;
		case GHASH:
			printk(KERN_INFO "Running test for ghash\n");
			break;
		case SHA256:
			printk(KERN_INFO "Running test for sha256\n");
			sha_test(0, "sha256");
			break;
		case SHA512:
			printk(KERN_INFO "Running test for sha512\n");
			sha_test(1, "sha512"); 
			break;
		default:
			printk("Invalid algorithm choice one to seven\n");
			return -EINVAL;
	}

	/* We don't want to rmmod the module always so let's do like tcrypt does */
	return -EINVAL;
}

static void __exit cryptotest_exit(void)
{
	printk(KERN_INFO "Leaving...\n");
}


module_init(cryptotest_init);
module_exit(cryptotest_exit);

module_param(alg, int, 0);
MODULE_LICENSE("GPL");
