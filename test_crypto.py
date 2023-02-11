
import hashlib
import subprocess
import string
import random
import sha3

sha3.Dprint = lambda *args, **kwargs: None

class CHECKER():
    def __init__(self, name, exe):
        self.name = name
        self._my_hash = exe

    def rand_str(length):
        return ''.join(random.choice(string.ascii_lowercase) for _ in range(length))

    def lib_hash(self, x):
        return self._lib_hash(x.encode('ascii')).hexdigest()

    def my_hash(self, x):
        #return sha3.sha256(x)
        proc = subprocess.run([f'./build/{self._my_hash}.elf', x], capture_output=True, check=True)
        return proc.stdout.decode('ascii').strip()

    def check(self, case):
        my, lib = None, None
        try:
            my = self.my_hash(case)
            lib = self.lib_hash(case)
            if my != lib:
                raise ValueError('fail')
        except Exception as e:
            print(f'test fall with exception ({e})')
            print(f"{self.name}:case: '{case}'")
            print(f'my ({my}) != lib ({lib})')
            raise e

class MD5_checker(CHECKER):
    def __init__(self):
        super().__init__('MD5', 'md5')
        self._lib_hash = hashlib.md5

class SHA3_checker(CHECKER):
    def __init__(self, size):
        super().__init__('', '')
        if size == 224:
            self._my_hash = 'sha224'
            self._lib_hash = hashlib.sha3_224
            self.name = 'SHA224'
        elif size == 256:
            self._my_hash = 'sha256'
            self._lib_hash = hashlib.sha3_256
            self.name = 'SHA256'
        elif size == 384:
            self._my_hash = 'sha384'
            self._lib_hash = hashlib.sha3_384
            self.name = 'SHA384'
        elif size == 512:
            self._my_hash = 'sha512'
            self._lib_hash = hashlib.sha3_512
            self.name = 'SHA512'
        else:
            raise ValueError(f'size {size} is invalid')


class SHA224_checker(SHA3_checker):
    def __init__(self):
        super().__init__(224)

class SHA256_checker(SHA3_checker):
    def __init__(self):
        super().__init__(256)

class SHA384_checker(SHA3_checker):
    def __init__(self):
        super().__init__(384)

class SHA512_checker(SHA3_checker):
    def __init__(self):
        super().__init__(512)

def main():
    checkers = [MD5_checker, SHA224_checker, SHA256_checker, SHA384_checker, SHA512_checker]
    checkers = [SHA256_checker, SHA224_checker, SHA512_checker]

    for Checker in checkers:
        print('===================')
        checker = Checker()
        case = ''
        for i in range(1, 512 + 1):
            case += str(i % 10)
            checker.check(case)
        print(f'{checker.name} simple tests OK')

        for i in range(10000):
            length = random.randint(0, 55)
            case = CHECKER.rand_str(length)
            checker.check(case)
        print(f'{checker.name} small tests OK')

        for i in range(5000):
            length = random.randint(0, 256)
            case = CHECKER.rand_str(length)
            checker.check(case)
        print(f'{checker.name} medium tests OK')

        for i in range(1000):
            length = random.randint(0, 10000)
            case = CHECKER.rand_str(length)
            checker.check(case)
        print(f'{checker.name} large tests OK')

if __name__ == '__main__':
    main()


