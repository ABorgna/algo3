#!/usr/bin/env python3

from random import random
import argparse
import sys


def genMaze(n,m,p):

    maze = [['.'] * n for _ in range(m)]

    for y in range(m):
        for x in range(n):
            if x == 0 or y == 0 or x == n-1 or y == m-1:
                maze[y][x] = '#'
                continue

            up = maze[y-1][x] != '.'
            left = maze[y][x-1] != '.'
            diag = maze[y-1][x-1] != '.'

            if up and left and diag:
                continue

            if up and left:
                maze[y][x] = '#'
                continue

            if up or left:
                if random() < p:
                    maze[y][x] = '#'
                continue

            if random() < p/4:
                maze[y][x] = '#'

    for row in maze:
        print(''.join(row))


if __name__ == "__main__":

    parser = argparse.ArgumentParser(
            description="Generate a map with size MxN")

    parser.add_argument('n', type=int)
    parser.add_argument('m', type=int)
    parser.add_argument('p', nargs='?', type=float, default=0.5)

    args = parser.parse_args()

    genMaze(args.n, args.m, args.p)

