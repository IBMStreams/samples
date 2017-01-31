#ifndef OPENSSLWRAPPER_H
#define OPENSSLWARPPER_H
#include <SPL/Runtime/Type/Blob.h>

namespace openssl_for_spl {


SPL::blob aesencrypt(const SPL::blob &key, const SPL::blob& input_data);

SPL::blob aesdecrypt(const SPL::blob& key, const SPL::blob &input_data);
}

#endif
