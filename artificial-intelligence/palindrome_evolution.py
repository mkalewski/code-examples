#!/usr/bin/env python3


import random
import string
import time

from argparse import ArgumentDefaultsHelpFormatter
from argparse import ArgumentParser


class PalindromeEvolution(object):
    """Evolve a palindrome of the given length."""

    __POPULATION_SIZE = 20

    def __init__(self, length):
        self.__population = self.__initialise(length)

    def __initialise(self, length):
        population = []
        for _ in range(0, PalindromeEvolution.__POPULATION_SIZE):
            dna = [random.choice(string.ascii_letters)
                   for gene in range(0, length)]
            fitness = self.__fitness(dna)
            sex = 'male' if random.randint(0, 1) else 'female'
            population.append({'dna': dna, 'fitness': fitness, 'sex': sex})
        return population

    def __fitness(self, dna):
        if dna == dna[::-1]:
            return 0
        fitness = 0
        length = len(dna) - 1
        for gene in range(0, int(len(dna) / 2)):
            diff = abs(ord(dna[gene]) - ord(dna[length - gene]))
            fitness += 100 + diff if diff > 0 else 0
        return fitness

    def __parent_selection(self, sex):
        while True:
            parent = int(random.random() * random.random() *
                         (len(self.__population) - 1))
            if self.__population[parent]['sex'] == sex:
                return self.__population[parent]

    def __recombine_dna(self, male, female):
        dna = male['dna'][:]
        start = random.randint(0, len(female['dna']) - 1)
        stop = random.randint(0, len(female['dna']) - 1)
        if start > stop:
            stop, start = start, stop
        dna[start:stop] = female['dna'][start:stop]
        return dna

    def __mutate_dna(self, dna):
        mutated_dna = dna[:]
        gene = random.randint(0, len(dna) - 1)
        mutated_dna[gene] = random.choice(string.ascii_letters)
        return mutated_dna

    def __reproduce(self, male, female):
        offsprings = []
        for _ in range(0, int(random.random() * random.random() * 9) + 1):
            dna = self.__mutate_dna(self.__recombine_dna(male, female))
            offsprings.append({'dna': dna, 'fitness': self.__fitness(dna)})
        offsprings.sort(key=lambda offspring: offspring['fitness'])
        return offsprings[0]

    def __generational_change(self, offspring):
        self.__population_sort()
        if offspring['fitness'] < self.__population[-1]['fitness']:
            offspring['sex'] = self.__population[-1]['sex']
            self.__population[-1] = offspring

    def __population_sort(self):
        self.__population.sort(key=lambda individual: individual['fitness'])

    def evolve(self):
        generation = 0
        while True:
            self.__population_sort()
            if self.__population[0]['fitness'] == 0:
                return ''.join(self.__population[0]['dna'])
            for _ in range(0, int(len(self.__population) * 0.6)):
                male = self.__parent_selection('male')
                female = self.__parent_selection('female')
                self.__generational_change(self.__reproduce(male, female))
            generation += 1


def main():
    parser = ArgumentParser(description=PalindromeEvolution.__doc__,
                            formatter_class=ArgumentDefaultsHelpFormatter)
    parser.add_argument('-length', '--l', type=int, default=64,
                        help='palindrome length')
    palindrome_evolution = PalindromeEvolution(vars(parser.parse_args())['l'])
    start_time = time.time()
    palindrome = palindrome_evolution.evolve()
    print('{0}\t{1:4f} seconds'.format(palindrome, time.time() - start_time))


if __name__ == '__main__':
    main()
