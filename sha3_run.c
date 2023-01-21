
#include <crypto.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char **argv)
{
    if (argc != 2) {
        return 1;
    }

    const char *hash = sha3_hexdigest(sha3_256(argv[1], strlen(argv[1])), 256);
    printf("%s\n", hash);
    return 0;
}
