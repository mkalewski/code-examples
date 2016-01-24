#!/usr/bin/env python3
"""https://github.com/mkalewski/code-examples/blob/master/LICENSE.txt"""


import multiprocessing
import random
import time

from argparse import ArgumentDefaultsHelpFormatter
from argparse import ArgumentParser
from math import sqrt


class PiMoneCarlo(object):
    """Approximate the value of Pi with the use of the Monte Carlo method."""

    def __init__(self, points_number):
        self.__points_number = int(points_number)

    @classmethod
    def in_circle(cls, x, y):
        return sqrt((x*x) + (y*y)) <= 1

    @classmethod
    def pi_monte_carlo(cls, points_number):
        return sum(1 for _ in range(points_number)
                   if PiMoneCarlo.in_circle(
                       random.uniform(-1, 1), random.uniform(-1, 1)))

    def compute(self):
        cpu_count = multiprocessing.cpu_count()
        parts = [int(self.__points_number/cpu_count) for _ in range(cpu_count)]
        pool = multiprocessing.Pool(processes=cpu_count)
        count = pool.map(PiMoneCarlo.pi_monte_carlo, parts)
        return 4.0 * (sum(count) / self.__points_number)


def main():
    parser = ArgumentParser(description=PiMoneCarlo.__doc__,
                            formatter_class=ArgumentDefaultsHelpFormatter)
    parser.add_argument('-points_number', '--pn', type=int, default=1000000,
                        help='points number')
    pi = PiMoneCarlo(vars(parser.parse_args())['pn'])
    start_time = time.time()
    pi_value = pi.compute()
    print('{0}\t({1:4f} seconds, {2} processes)'.format(
        pi_value, time.time() - start_time, multiprocessing.cpu_count()))


if __name__ == '__main__':
    main()
