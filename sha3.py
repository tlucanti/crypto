
VERBOSE = False

def ROTL64(_v, i):
    return ((_v << i) & 0xffffffffffffffff) | (_v >> (64 - i))

def prt(r, name, a):
    if not VERBOSE:
        return
    print(f'{r}:{name} ----------', end='')
    for i in range(25):
        if (i % 5 == 0):
            print()
        print(f" {a[i]:20}", end='')
    print()

def mod5(x):
    return x % 5

def permutate(st, l):
    BC = [0] * 5
    perm_values = [
        10,  7, 11, 17, 18,
         3,  5, 16,  8, 21,
        24,  4, 15, 23, 19,
        13, 12,  2, 20, 14,
        22,  9,  6,  1
    ]
    rotl_values = [
         1,  3,  6, 10, 15,
        21, 28, 36, 45, 55,
         2, 14, 27, 41, 56,
         8, 25, 43, 62, 18,
         39, 61, 20, 44
    ]

    iota_const = [
        0x0000000000000001, 0x0000000000008082,
        0x800000000000808a, 0x8000000080008000,
        0x000000000000808b, 0x0000000080000001,
        0x8000000080008081, 0x8000000000008009,
        0x000000000000008a, 0x0000000000000088,
        0x0000000080008009, 0x000000008000000a,
        0x000000008000808b, 0x800000000000008b,
        0x8000000000008089, 0x8000000000008003,
        0x8000000000008002, 0x8000000000000080,
        0x000000000000800a, 0x800000008000000a,
        0x8000000080008081, 0x8000000000008080,
        0x0000000080000001, 0x8000000080008008,
    ]

    prt(0, 'start', st)
    for round in range(12 + 2*l):
        # theta
        for i in range(5):
            BC[i] = st[i] ^ st[i + 5] ^ st[i + 10] ^ st[i + 15] ^ st[i + 20]
        for i in range(5):
            tmp = BC[mod5(i + 4)] ^ ROTL64(BC[mod5(i + 1)], 1)
            for j in range(0, 25, 5):
                st[i + j] ^= tmp
        prt(round, 'theta', st)

        # rho + pi
        temp = st[1];
        for i in range(24):
            save = st[perm_values[i]]
            st[perm_values[i]] = ROTL64(temp, rotl_values[i])
            temp = save
        prt(round, 'rho+pi', st)

        # chi
        for i in range(0, 25, 5):
            BC = st[i:i+5]
            for j in range(5):
                st[i + j] ^= ~BC[mod5(j + 1)] & BC[mod5(j + 2)];
        prt(round, 'chi', st)

        # iota
        st[0] ^= iota_const[round];
        prt(round, 'iota', st)
    return st


def byte2long(A):
    a = []
    assert(len(a) % 4 == 0)
    for i in range(0, len(A), 8):
        a.append(int.from_bytes(A[i:i+8], 'little'))
    return a

def long2byte(A):
    b = bytearray()
    for a in A:
        b += a.to_bytes(8, 'little')
    return b

def sha3(msg, r, c, d):

    assert(r + c == 200)
    l = 6
    b = 5

    # stage 1
    # padding
    msg = bytearray(msg, 'ascii')
    msg.append(0x06)
    while len(msg) % r != 0:
        msg.append(0x0)
    msg[-1] |= 0x80

    # stage 2
    # absorbtion
    S = [0] * (25 * 8)
    assert(len(msg) % r == 0)
    for block in range(0, len(msg), r):
        for i in range(r):
            try:
                S[i] ^= msg[block + i]
            except Exception as e:
                print(f' len({len(msg)}), block({block}),  r({r}), i({i}), I({block * r + i}), {msg}')
                raise e
        S = long2byte(permutate(byte2long(S), l))

    # stage 3
    # squeezing
    #Z = S[:r]
    #while len(Z) < d:
    #    S = long2byte(permutate(byte2long(S), l))
    #    Z += S[:r]
    Z = S[:d]

    # stage 4
    # hexdigest
    ans = ''
    for a in Z:
        ans += f'{a:02x}'
    return ans

def sha224(msg):
    return sha3(msg, r=144, c=56, d=28)

def sha256(msg):
    return sha3(msg, r=136, c=64, d=32)

def sha384(msg):
    return sha3(msg, r=104, c=96, d=48)

def sha512(msg):
    return sha3(msg, r=72, c=128, d=64)

if __name__ == '__main__':
    VERBOSE = True
    print(sha256('1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456'))

