
#ifndef __SHA3_COMMON_H__
# define __SHA3_COMMON_H__


const unsigned char *sha3(const char *message_ptr,
                          const size_t len,
                          const unsigned short r,
                          const unsigned short d);

# define MOD5(__val) ((__val) % 5)

#endif /* __SHA3_COMMON_H__ */

