#include <openssl/evp.h>
#include "opensslwrapper.h"
#include <iostream>

#define BYTES_PER_BLOCK 16

namespace openssl_for_spl {
void crypto(const unsigned char * key, const unsigned char * input, int len, unsigned char* output,int * outputLen, bool encrypt) {

	EVP_CIPHER_CTX ctx;
	EVP_CIPHER_CTX_init(&ctx);
	EVP_CipherInit_ex(&ctx,EVP_aes_128_cbc(),NULL,key,NULL,encrypt ? 1 :0);
	int tmpLen =0;
	if (!EVP_CipherUpdate(&ctx,output, &tmpLen,input, len)) {
		EVP_CIPHER_CTX_cleanup(&ctx);
		*outputLen=0;
		return;
	}
	*outputLen=tmpLen;
	int finalBytes = 0;
	if (!EVP_CipherFinal_ex(&ctx,output+tmpLen,&finalBytes)) {
		*outputLen=0;
		return;
	}
	*outputLen += finalBytes;
	EVP_CIPHER_CTX_cleanup(&ctx);

}


SPL::blob aesencrypt(const SPL::blob &key, const SPL::blob& input_data) {
	int outputLen = 0;
	const int len = input_data.getSize();
	unsigned char  output[len + BYTES_PER_BLOCK];
	crypto(key.getData(),input_data.getData(),input_data.getSize(),&(output[0]),&outputLen,true);
	SPL::blob myblob(output,outputLen);;
    return myblob;
}

SPL::blob aesdecrypt(const SPL::blob& key, const SPL::blob &input_data) {
	int outputLen = 0;
	const int len = input_data.getSize();
	// allocate a little extra space
	unsigned char output[len+BYTES_PER_BLOCK];
	 crypto(key.getData(),input_data.getData(),input_data.getSize(),&(output[0]), &outputLen,false);
	SPL::blob myblob(output,outputLen);
	return myblob;
}
}
