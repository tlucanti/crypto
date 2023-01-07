
import hashlib
import subprocess
import string
import random

def lib_md5(x):
    return hashlib.md5(x.encode('ascii')).hexdigest()

def my_md5(x):
    proc = subprocess.run(['./md5', x], capture_output=True, check=True)
    return proc.stdout.decode('ascii').strip()

def rand_str(length):
    return ''.join(random.choice(string.ascii_lowercase) for _ in range(length))

def check(case):
    my = my_md5(case)
    lib = lib_md5(case)
    if (my != lib):
        print(f"case: '{case}'")
        print(f'my ({my}) != lib ({lib})')
        raise ValueError('fail')

def main():
    case = ''
    for i in range(1, 512 + 1):
        case += str(i % 10)
        check(case)
    print('simple tests OK')

    for i in range(10000):
        length = random.randint(0, 55)
        case = rand_str(length)
        check(case)
    print('small tests OK')

    for i in range(5000):
        length = random.randint(0, 256)
        case = rand_str(length)
        check(case)
    print('medium tests OK')

    for i in range(1000):
        length = random.randint(0, 10000)
        case = rand_str(length)
        check(case)
    print('large tests OK')

if __name__ == '__main__':
    main()


