
#include <crypto.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char **argv)
{
    if (argc != 2) {
        return 1;
    }
    const char *hash = md5_hexdigest(md5(argv[1], strlen(argv[1])));
    printf("%s\n", hash);
    return 0;
}
