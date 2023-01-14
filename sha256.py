
def ROTL64(_v, i):
    return ((_v << 1) & 0xffffffffffffffff) | (_v >> (64 - i))

def prt(r, a):
    print(r, '----------', end='')
    for i in range(25):
        if (i % 5 == 0):
            print()
        print(f" {a[i]:20}", end='')
    print()

def permutate(a, l):
    for round in range(12 + 2*l):
        prt(round, a)
        # theta
        C = [0] * 5
        D = [0] * 5
        for i in range(5):
            C[i] = a[i] ^ a[i + 5] ^ a[i + 10] ^ a[i + 15] ^ a[i + 20]
        for i in range(5):
            D[i] = C[(i + 4) % 5] ^ ROTL64(C[(i + 1) % 5], 1)
            for j in range(0, 25, 5):
                a[j] ^= D[i]

def byte2long(A):
    a = []
    assert(len(a) % 4 == 0)
    for i in range(0, len(A), 8):
        a.append(int.from_bytes(A[i:i+8], 'little'))
    return a

def sha256(msg):
    r = 136
    c = 64
    l = 6
    b = r + c

    # stage 1
    # expand
    msg = bytearray(msg, 'ascii')
    if len(msg) % r == r - 1:
        msg.append(0x81)
    else:
        msg.append(0x80)
        while len(msg) % r != r - 1:
            msg.append(0x0)
        msg.append(0x1)
    print(len(msg), msg)

    # stage 2
    # absorbtion
    S = [0] * b
    assert(len(msg) % r == 0)
    for block in range(0, len(msg), r):
        for i in range(r):
            S[i] ^= msg[block * r + i]
        permutate(byte2long(S), l)



if __name__ == '__main__':
    print(sha256('123'))

