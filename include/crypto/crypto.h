
#ifndef CRYPTO_H
# define CRYPTO_H

# include <stddef.h>
# ifdef __cplusplus
#  define __static /* nothing */
# else
#  define __static static
# endif

extern "C" { // }

const unsigned char *md5(const char *__restrict message, size_t len);
const char *md5_hexdigest(const unsigned char hash[__static 16]);

const unsigned char *sha3_224(const char *__restrict message, size_t len);
const unsigned char *sha3_256(const char *__restrict message, size_t len);
const unsigned char *sha3_384(const char *__restrict message, size_t len);
const unsigned char *sha3_512(const char *__restrict message, size_t len);
const char *sha3_hexdigest(const unsigned char *__restrict hash, size_t len);
const char *sha3_224_hexdigest(const unsigned char hash[__static 28]);
const char *sha3_256_hexdigest(const unsigned char hash[__static 32]);
const char *sha3_384_hexdigest(const unsigned char hash[__static 48]);
const char *sha3_512_hexdigest(const unsigned char hash[__static 64]);

} /* extern C */

#endif /* CRYPTO_H */

