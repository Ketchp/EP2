#!/usr/bin/env python3

import os
import sys
import time
from uuid import uuid4
from enum import Enum
from pathlib import Path
from typing import Optional
from subprocess import Popen, TimeoutExpired
from tempfile import TemporaryFile, TemporaryDirectory

from generator import WRAPPED_TESTS


TIME_LIMIT = 10
KILL_LIMIT = 2 * TIME_LIMIT
SHOW_WA_DIFF = True
CONTINUE_AFTER_FAIL = False
ShOW_TIME_OK = True
SUBJECT_DIR = Path(__file__).parent


class RESULT(Enum):
    OK = 'OK'
    WA = 'wrong answer'
    CTE = 'compile time error'
    RTE = 'run time error'
    TLE = 'time limit exceeded'


class LANGUAGE(Enum):
    RAW = 'raw binary'
    CPP = 'C++'


class CompileTimeError(Exception):
    pass


class SubjectWrapper:
    CXX_FLAGS = ('-std=c++20', "-Wall", "-O3")

    def __init__(self, filename, lang: Optional[LANGUAGE] = None):
        filename = Path(filename).resolve()
        if lang is None:
            if filename.suffix == '':
                lang = LANGUAGE.RAW
            elif filename.suffix in ['.cpp', '.c++']:
                lang = LANGUAGE.CPP
            else:
                raise NotADirectoryError()
        self.lang = lang
        self.temp_dir: Optional[TemporaryDirectory] = None
        self.source: Path = filename
        self.bin_name: Optional[Path] = None
        self.compilation_result = RESULT.OK

    def get_process(self, f_in, f_out):
        if self.compilation_result == RESULT.CTE:
            raise CompileTimeError()

        if self.lang == LANGUAGE.CPP:
            return Popen([self.bin_name], stdin=f_in, stdout=f_out)
        elif self.lang == LANGUAGE.RAW:
            return Popen([self.source], stdin=f_in, stdout=f_out)
        else:
            raise NotImplementedError()

    def __enter__(self):
        if self.lang == LANGUAGE.CPP:
            self.temp_dir = TemporaryDirectory()
            self.bin_name = Path(self.temp_dir.name) / str(uuid4())
            p = Popen(['g++', *self.CXX_FLAGS, self.source, '-o', self.bin_name])
            p.wait()
            if p.returncode != 0:
                self.compilation_result = RESULT.CTE
        return self

    def __exit__(self, exc_type, exc_val, exc_tb):
        if self.temp_dir:
            self.temp_dir.cleanup()
            self.temp_dir = None


def run_test(factory: SubjectWrapper, test_fn, f_in, f_sample, f_test):
    test_fn(f_in=f_in, f_out=f_sample)

    ts = time.monotonic()
    try:
        p = factory.get_process(f_in, f_test)
    except CompileTimeError:
        print('compilation failed')
        return RESULT.CTE

    killed = False
    try:
        p.wait(KILL_LIMIT)
    except TimeoutExpired:
        p.kill()
        killed = True
    elapsed = time.monotonic() - ts

    sample_out = f_sample.read()
    f_test.seek(0)
    subject_out = f_test.read()

    p.wait()  # kill may take some time
    if elapsed > TIME_LIMIT:
        state = RESULT.TLE
    elif p.returncode != 0:
        state = RESULT.RTE
    elif subject_out != sample_out:
        state = RESULT.WA
    else:
        state = RESULT.OK

    print(state.value, end=' ')

    if state == RESULT.RTE:
        print(f'Exit code: {p.returncode}')
    if state == RESULT.WA and SHOW_WA_DIFF:
        print()
        print('Sample:')
        print(sample_out)
        print('Program:')
        print(subject_out)

    if state == RESULT.TLE:
        if killed:
            print(f'killed after {KILL_LIMIT}s')
        else:
            print(f'{elapsed:.3f}s > {TIME_LIMIT:.3f}s')

    if state == RESULT.OK and ShOW_TIME_OK:
        print(f'in {elapsed:.3f}s.')

    return state


def main(filename):
    with SubjectWrapper(filename) as process_factory:
        for idx, test_fn in enumerate(WRAPPED_TESTS, start=1):
            print(f'Running test {idx}({test_fn.__name__[len("test_"):]}): ', end='')
            with TemporaryFile('w+') as f_in:
                with TemporaryFile('w+') as f_out_sample:
                    with TemporaryFile('w+') as f_out_program:
                        res = run_test(process_factory, test_fn, f_in, f_out_sample, f_out_program)
                        if res != RESULT.OK and not CONTINUE_AFTER_FAIL:
                            break


if __name__ == '__main__':
    try:
        os.nice(-19)
        cpu_set = os.sched_getaffinity(0)
        cpu_set = {next(iter(cpu_set))}  # pick any CPU, but only one
        os.sched_setaffinity(0, cpu_set)
    except PermissionError:
        print('Could not set niceness, time measurements will be less accurate.', file=sys.stderr)
        print(f'Consider using tester.c wrapper that sets needed capabilities.', file=sys.stderr)
        print(f'Use `make` to create tester binary wrapper.', file=sys.stderr)

    main(filename=SUBJECT_DIR / 'solution.cpp')
