
#include "sha3.c"

int main()
{
    keccak_state *st;
    uint8_t ans[257];
    const char *msg = "123";

    keccak_init(&st, 64, 24);
    keccak_absorb(st, (const uint8_t *)msg, strlen(msg));
    assert(keccak_digest(st, ans, 32, 0x06) == 0);
    for (int i = 0; i < 32; ++i) {
        printf("%02x", ans[i]);
    }
    printf("\n");
}
