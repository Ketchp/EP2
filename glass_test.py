import os
import sys
import random
from subprocess import Popen, PIPE
from pathlib import Path


def _test_generator(length, c_max):
    chars = [chr(ord('a') + i) for i in range(c_max)]

    while True:
        yield random.choices(chars, k=length)


def _solve_test(test) -> int:
    poss = [(test[idx:] + test[:idx], idx) for idx in range(len(test))]
    return sorted(poss)[0][1] + 1


def main():
    length = 5
    c_max = ord('z') - ord('a')
    count = 1000

    if len(sys.argv) >= 2:
        length = int(sys.argv[1])
    if len(sys.argv) >= 3:
        c_max = int(sys.argv[2])
    if len(sys.argv) >= 4:
        count = int(sys.argv[3])

    p = Popen(Path(__file__).parent / 'cmake-build-debug' / 'glass',
              stdin=PIPE, stdout=PIPE)

    p.stdin.write(b'1000000\n')

    tests = _test_generator(length, c_max)
    passed = 0
    while passed != count:
        test = next(tests)

        s = ''.join(test)
        p.stdin.write(f'{s}\n'.encode())
        p.stdin.flush()

        solution = _solve_test(s)

        reported = p.stdout.readline().decode()

        if int(reported) != solution:
            print(f'Failed on test: "{s}", corr = {solution}, rep = {reported}')
            return

        passed += 1
        if passed % 1000 == 0:
            print(f'Passed {passed}')


if __name__ == '__main__':
    main()
