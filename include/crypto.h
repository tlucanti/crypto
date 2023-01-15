
#ifndef CRYPTO_H
# define CRYPTO_H

# include <stddef.h>

const unsigned char *md5(const char *message, size_t len);
const char *md5_hexdigest(const unsigned char hash[static 16]);

#endif /* CRYPTO_H */

