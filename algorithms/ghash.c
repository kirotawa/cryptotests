#include <crypto/hash.h>
#include <linux/scatterlist.h>
#include <linux/string.h>
#include <linux/gfp.h>

#include <cryptotest.h>

#define GHASH_DIGEST_SIZE 	16
#define GHASH_KEY_SIZE		(16)
#define BUF_SIZE		8

struct ghash_result {
	struct completion completion;
	int err;
};

/* Based on/copied from kernel/crypto/testmgr.c */
void ghash_complete(struct crypto_async_request *req, int err)
{
	struct ghash_result *res = req->data;
	
	if (err == -EINPROGRESS)
		return;
	res->err = err;
	complete(&res->completion);
}

int wait_async_op(struct ghash_result *gr, int ret)
{
	if (ret == -EINPROGRESS || ret == -EBUSY) {
		wait_for_completion(&gr->completion);
		reinit_completion(&gr->completion);
		ret = gr->err;
	}
	return ret;
}

int alloc_buffer(u8 *buf[BUF_SIZE])
{
	int i;

	/* set a buffer */
	for (i = 0; i < BUF_SIZE; i++) {
		buf[i] = (void *)__get_free_page(GFP_KERNEL);
		if (!buf[i]) {
			while (i-- > 0)
				free_page((unsigned long)buf[i]);
			return -ENOMEM;
		}
	}
	return 0;
}

void free_buffer(u8 *buf[BUF_SIZE])
{
	int i;

	for (i = 0; i <  BUF_SIZE; i++)
		free_page((unsigned long)buf[i]);
}

int ghash_test(char *msg, int msg_size, char *_key)
{
	struct crypto_ahash *tfm;
	struct ahash_request *req;
	struct scatterlist sg[8];
	struct ghash_result g_result;
	u8 key[GHASH_KEY_SIZE];
	u8 result[GHASH_DIGEST_SIZE] = {0};
	u8 *buffer[BUF_SIZE];
	void *ghash_buffer;
	int rc = 0;
	int ret = 0;
	char *plaintext = kmalloc(msg_size, GFP_KERNEL);
	int psize = msg_size;

	memcpy(key, _key, GHASH_KEY_SIZE);
	memcpy(plaintext, msg, msg_size);

	if (alloc_buffer(buffer)) {
		printk("Error buffer\n");
		goto free_buffer;
	}

	tfm = crypto_alloc_ahash("ghash", 0, 0);
	if (IS_ERR(tfm)) {
		rc = PTR_ERR(tfm);
		printk(KERN_ERR "Failed to allocate transformation: %d\n", rc);
		return rc;	
	}	

	init_completion(&g_result.completion);
	
	req = ahash_request_alloc(tfm, GFP_KERNEL);
	if (!req) {
		printk(KERN_ERR "Failed to allocate request for\n");
		goto err;
	}
	ahash_request_set_callback(req, CRYPTO_TFM_REQ_MAY_BACKLOG,
					ghash_complete, &g_result);

	ghash_buffer = buffer[0];
	memcpy(ghash_buffer, plaintext, psize);
	sg_init_one(&sg[0], ghash_buffer, psize);

	crypto_ahash_clear_flags(tfm, ~0);
	
	ret = crypto_ahash_setkey(tfm, key, GHASH_KEY_SIZE);
	if (ret) {
		printk(KERN_ERR "Failed to setkey\n");
		return -EINVAL;
	}
	
	ahash_request_set_crypt(req, sg, result, psize);

	ret = wait_async_op(&g_result, crypto_ahash_init(req));
	if (ret) {
		pr_err("Failed on init\n");
		goto err;
	}
	ret = wait_async_op(&g_result, crypto_ahash_update(req));
	if (ret) {
		pr_err("Update failed\n");
		goto err;
	}
	ret = wait_async_op(&g_result, crypto_ahash_final(req));
	if (ret) {
		pr_err("Final failed\n");
		goto err;
	}

	hexdump(result);

err:
	crypto_free_ahash(tfm);

free_buffer:	
	free_buffer(buffer);

	return ret;
}
