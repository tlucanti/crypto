
#include <common.h>

typedef union {
    void        *ptr;
    uint64_t    as_64vec[8];
    uint32_t    as_32vec[16];
    uint8_t     as_8vec[64];
} chunk_t;

typedef union {
    void        *ptr;
    uint32_t    as_32vec[4];
    uint64_t    as_64vec[2];
} hash_t;

void md5_step(chunk_t chunk, hash_t hash)
{
    const static uint32_t T[64] = {
        3614090360, 3905402710,  606105819, 3250441966,
        4118548399, 1200080426, 2821735955, 4249261313,
        1770035416, 2336552879, 4294925233, 2304563134,
        1804603682, 4254626195, 2792965006, 1236535329,
        4129170786, 3225465664,  643717713, 3921069994,
        3593408605,   38016083, 3634488961, 3889429448,
         568446438, 3275163606, 4107603335, 1163531501,
        2850285829, 4243563512, 1735328473, 2368359562,
        4294588738, 2272392833, 1839030562, 4259657740,
        2763975236, 1272893353, 4139469664, 3200236656,
         681279174, 3936430074, 3572445317,   76029189,
        3654602809, 3873151461,  530742520, 3299628645,
        4096336452, 1126891415, 2878612391, 4237533241,
        1700485571, 2399980690, 4293915773, 2240044497,
        1873313359, 4264355552, 2734768916, 1309151649,
        4149444226, 3174756917,  718787259, 3951481745
    };
    const static uint32_t K1[16] = { 0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15};
    const static uint32_t K2[16] = { 1,  6, 11,  0,  5, 10, 15,  4,  9, 14,  3,  8, 13,  2,  7, 12};
    const static uint32_t K3[16] = { 5,  8, 11, 14,  1,  4,  7, 10, 13,  0,  3,  6,  9, 12, 15,  2};
    const static uint32_t K4[16] = { 0,  7, 14,  5, 12,  3, 10,  1,  8, 15,  6, 13,  4, 11,  2,  9};

    const static uint32_t S1[16] = { 7, 12, 17, 22,  7, 12, 17, 22,  7, 12, 17, 22,  7, 12, 17, 22};
    const static uint32_t S1[16] = { 5,  9, 14, 20,  5,  9, 14, 20,  5,  9, 14, 20,  5,  9, 14, 20};
    const static uint32_t S1[16] = { 4, 11, 16, 23,  4, 11, 16, 23,  4, 11, 16, 23,  4, 11, 16, 23};
    const static uint32_t S1[16] = { 6, 10, 15, 21,  6, 10, 15, 21,  6, 10, 15, 21,  6, 10, 15, 21};

    uint32_t a, b, c, d;
    const uint32_t iA = 0x67452301;
    const uint32_t iB = 0xefcdab89;
    const uint32_t iC = 0x98badcfe;
    const uint32_t iD = 0x10325476;
}

const char *md5(const char *message, size_t len)
{
    chunk_t chunk;
    unsigned char remaining;
    static hash_t result = { .as_32vec = { 0x67452301, 0xefcdab89, 0x98badcfe, 0x10325476 } };
    hash_t hash;

    for (size_t offset = 0; offset < len; offset += 64) {
        memcpy(chunk.ptr, message + offset, 64);
        md5_step(chunk, hash);
        result.as_64vec[0] += hash.as_64vec[0];
        result.as_64vec[1] += hash.as_64vec[1];
    }
    remaining = len % 64;
    if (remaining <= 55) {
        memset(chunk.ptr, 0, 64);
        memset(chunk.ptr, message + offset, remaining);
        chunk.as_8vec[remaining] = 0x80;
        chunk.as_64vec[7] = len;
        md5_step(chunk);
        result.as_64vec[0] += hash.as_64vec[0];
        result.as_64vec[1] += hash.as_64vec[1];
    } else {
        chunk.as_64vec[7] = 0;
        chunk.as_8vec[remaining] = 0x80;
        md5_step(chunk);
        result.as_64vec[0] += hash.as_64vec[0];
        result.as_64vec[1] += hash.as_64vec[1];
        memset(chunk.ptr, 0, 56);
        chunk.as_64vec[7] = len;
        md5_step(chunk);
        result.as_64vec[0] += hash.as_64vec[0];
        result.as_64vec[1] += hash.as_64vec[1];
    }
    return result.ptr;
}

