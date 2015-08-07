#ifndef OPENSSLWRAPPER_H
#define OPENSSLWARPPER_H
#include <openssl/evp.h>
#include <SPL/Runtime/Type/Blob.h>

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
	if (!EVP_CipherFinal_ex(&ctx,output+tmpLen,&tmpLen)) {
		*outputLen=0;
		return;
	}
	*outputLen += tmpLen;
	EVP_CIPHER_CTX_cleanup(&ctx);

}

SPL::blob aesencrypt(const SPL::blob &key, const SPL::blob& input_data) {
	SPLAPPTRC(L_DEBUG,"Encrypt input size " << input_data.getSize(),"opensslwrapper");
	int outputLen = 0;
	const int len = input_data.getSize();
	unsigned char  output[len+BYTES_PER_BLOCK];
	crypto(key.getData(),input_data.getData(),input_data.getSize(),&(output[0]),&outputLen,true);
    SPL::blob myblob(output,outputLen);;
    SPLAPPTRC(L_DEBUG,"Encrypt output size " << outputLen,"opensslwrapper");
    return myblob;
}

SPL::blob aesdecrypt(const SPL::blob& key, const SPL::blob &input_data) {
	SPLAPPTRC(L_DEBUG,"Decrypt input size " << input_data.getSize(),"opensslwrapper");
	int outputLen = 0;
	const int len = input_data.getSize();
	unsigned char output[len+BYTES_PER_BLOCK];
	 crypto(key.getData(),input_data.getData(),input_data.getSize(),&(output[0]), &outputLen,false);
	SPL::blob myblob(output,outputLen);
	SPLAPPTRC(L_DEBUG,"Decrypt output size " << outputLen,"opensslwrapper");
	return myblob;
}
}

#endif
