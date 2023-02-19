
#include <crypto.h>
#include <stdio.h>
#include <string.h>

#if !defined(SHA3_HASH_SIZE) || !defined(SHA3_HASH_FN)
# error "no hash size chosen"
#endif

int main(int argc, char **argv)
{
    const char *hash;

    if (argc != 2) {
        return 1;
    }
    hash = sha3_hexdigest(
        SHA3_HASH_FN(argv[1], strlen(argv[1])), SHA3_HASH_SIZE);
    printf("%s\n", hash);
    return 0;
}
