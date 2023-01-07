
def md5(msg):
    from math import sin

    def _csll(_x, _s):
        return (_x << _s) | (_x >> (32 - _s))

    def _round(_abcd, _k, _s, _i, _X, _T, _fun):
        _a, _b, _c, _d = _abcd
        _sh = (_a + _fun(_b, _c, _d) + _X[_k] + _T[_i]) & 0xffffffff
        _new_b = (_b + _csll(_sh, _s)) & 0xffffffff
        return _d, _new_b, _b, _c

    msg = bytearray(msg, 'ascii')
    length = len(msg) * 8

    # step 0
    # append 0x80
    msg.append(0x80)

    # step 1
    # pad message with zeros until its lenth != 56 mod 64
    while len(msg) % 64 != 56:
        msg.append(0x0)

    # step 2
    # append message length to the end of message
    msg += bytes(length.to_bytes(8, 'little'))

    # step 3
    # initialize buffers
    initA = 0x67452301
    initB = 0xefcdab89
    initC = 0x98badcfe
    initD = 0x10325476

    funF = lambda x, y, z: (x & y) | (~x & z)
    funG = lambda x, y, z: (x & z) | (~z & y)
    funH = lambda x, y, z: x ^ y ^ z
    funI = lambda x, y, z: y ^ (~z | x)

    T = [int(abs(sin(i + 1)) * 2**32) & 0xffffffff for i in range(64)]
    K = [
        [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15],
        [1, 6, 11, 0, 5, 10, 15, 4, 9, 14, 3, 8, 13, 2, 7, 12],
        [5, 8, 11, 14, 1, 4, 7, 10, 13, 0, 3, 6, 9, 12, 15, 2],
        [0, 7, 14, 5, 12, 3, 10, 1, 8, 15, 6, 13, 4, 11, 2, 9]
    ]
    S = [
        [7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22],
        [5,  9, 14, 20, 5,  9, 14, 20, 5,  9, 14, 20, 5,  9, 14, 20],
        [4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23],
        [6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21]
    ]
    funcs = [funF, funG, funH, funI]
    ans = [initA, initB, initC, initD]

    # step 4
    # encryption rounds
    for offset in range(0, len(msg), 64):
        print('=================================================')
        chunk = msg[offset:offset + 64]
        chunk_pieces = [int.from_bytes(chunk[i:i+4], 'little') for i in range(0, len(chunk), 4)]
        #print(chunk)
        #print(ans)

        abcd = list(ans)
        for i in range(4):
            a, b, c, d = abcd
            for r in range(16):
                a, b, c, d = abcd
                print(f'{i} {r:2} ({a:10} {b:10} {c:10} {d:10})')
                print(f'K({K[i][r]}) S({S[i][r]}) I({i*16+r}) T({T[i*16+r]}) X({chunk_pieces[K[i][r]]})')
                abcd = _round(abcd, K[i][r], S[i][r], i * 16 + r, chunk_pieces, T, funcs[i])
        print(abcd)
        for i in range(4):
            ans[i] = (ans[i] + abcd[i]) & 0xffffffff

    ret = ''
    print(list(ans))
    for i in ans:
        little = i.to_bytes(4, 'little')
        big = int.from_bytes(little, 'big')
        ret += f'{big:04x}'
    return ret


print(md5('12345678901234567890123456789012345678901234567890123456789012345'))
