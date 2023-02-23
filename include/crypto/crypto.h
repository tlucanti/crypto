
#ifndef CRYPTO_H
# define CRYPTO_H

# include <stddef.h>

const unsigned char *md5(const char *__restrict message, size_t len);
const char *md5_hexdigest(const unsigned char hash[static 16]);

const unsigned char *sha3_224(const char *message, size_t len);
const unsigned char *sha3_256(const char *message, size_t len);
const unsigned char *sha3_384(const char *message, size_t len);
const unsigned char *sha3_512(const char *message, size_t len);
const char *sha3_hexdigest(const unsigned char *hash, size_t len);

#endif /* CRYPTO_H */

