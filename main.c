
#include <common.h>
#include <stdio.h>

int main()
{
    const char *msg = "1234567890";
    const char *hash = md5_to_hex(md5(msg, strlen(msg)));

    printf("%s\n", hash);
}
