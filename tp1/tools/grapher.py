#!/usr/bin/env python3

from pylab import log,loglog
import argparse
import numpy as np
import math
import matplotlib.colors as mplc
import matplotlib.ticker as mticker
import matplotlib.pyplot as plt
import sys

class Grapher:

    COLORS = [ (r/255., g/255., b/255.) for r,g,b in
             [(31, 119, 180), (174, 199, 232), (255, 127, 14), (255, 187, 120),
             (44, 160, 44), (152, 223, 138), (214, 39, 40), (255, 152, 150),
             (148, 103, 189), (197, 176, 213), (140, 86, 75), (196, 156, 148),
             (227, 119, 194), (247, 182, 210), (127, 127, 127), (199, 199, 199),
             (188, 189, 34), (219, 219, 141), (23, 190, 207), (158, 218, 229)]
    ]

    def __init__(self):
        plt.style.use('ggplot')

    def run(self, infile, outfile, mode, modevars, title='', xlabel='',
            xscale='linear', yscale='linear', polyfit=None, fitdegree=1,
            exitstatus=[]):

        points = []

        for line in infile:
            values = [int(x) for x in line.split()]

            # Last number is the function return value
            # Ignore this datapoint if non zero
            ret = values.pop()
            if len(exitstatus) and int(ret) not in exitstatus:
                continue

            # Next is the number of runs
            values.pop()

            # Minimum runtime, in nS
            time = values.pop()

            # What's left is the original variables
            x = None

            if mode == 'sum':
                x = sum(values)

            elif mode == 'simple':
                if len(values) != len(modevars):
                    print('Incorrect number of variables for mode "simple", expected',
                            len(values), 'but got', len(modevars), file=sys.stderr)
                    sys.exit(2)

                x = 0
                for mv, v in zip(modevars,values):
                    valid = True
                    use = False

                    if mv[0] == '+':
                        x += v
                        mv = mv[1:]
                    elif mv[0] == '*':
                        x = v if x == 0 else x * v
                        mv = mv[1:]

                    if len(mv):
                        if mv[0] == '>':
                            if v <= int(mv[1:]):
                                valid = False
                        elif mv[0] == '<':
                            if v >= int(mv[1:]):
                                valid = False
                        elif mv[0] == '!':
                            if v == int(mv[1:]):
                                valid = False
                        elif mv != '?':
                            if int(mv) != v:
                                valid = False

                    if not valid:
                        x = None
                        break

            if x is None:
                continue

            point = (x,time)
            points.append(point)

        points.sort()

        if len(points):
            self.plotTime(points, outfile, title, xlabel,
                    xscale, yscale, polyfit, fitdegree)
        else:
            print("No datapoints available", file=sys.stderr)


    # Graphs

    def plotTime(self, points, outfile, title, xlabel, xscale, yscale,
            polyfit, fitdegree):
        sets = []
        groups = []

        xs = [x for x,y in points]
        ys = [y for x,y in points]

        maxY = max(ys)

        # The original times are in ns
        unit = "s"
        if maxY < 2e3:
            unit = "ns"
        elif maxY < 2e6:
            unit = "us"
            ys = [float(y)/1e3 for y in ys]
            maxY = float(maxY)/1e3
        elif maxY < 2e9:
            unit = "ms"
            ys = [float(y)/1e6 for y in ys]
            maxY = float(maxY)/1e6
        elif maxY < 2e12:
            unit = "s"
            ys = [float(y)/1e9 for y in ys]
            maxY = float(maxY)/1e9

        # Plot the data
        fig, ax = plt.subplots()
        fig.set_size_inches((12,9))

        ax.plot(xs, ys, 'o', color=self.COLORS[0])

        if polyfit is not None:

            if polyfit == 'linear':
                scale = lambda y : y
                scaleback = lambda y : y
            elif polyfit == 'exp':
                scale = lambda y : log(y)
                scaleback = lambda y : math.e**y
            elif polyfit == 'log':
                scale = lambda y : math.e**y
                scaleback = lambda y : log(y)
            elif polyfit == 'loglog':
                scale = lambda y : math.e**(math.e**y)
                scaleback = lambda y : log(log(y))

            coef = np.polyfit(xs, [scale(y) for y in ys], fitdegree)

            fit = lambda x : scaleback(sum([a * x**(fitdegree-i)
                                 for (i,a) in enumerate(coef)]))
            ax.plot(xs, [fit(x) for x in xs], color=self.COLORS[2],
                    linewidth=2)

        ax.set_title(title)
        ax.set_xlabel(xlabel)
        ax.set_ylabel("Tiempo")
        ax.set_xscale(xscale)
        ax.set_yscale(yscale)
        ax.set_ylim(ymin=0)
        ax.yaxis.set_major_formatter(mticker.FormatStrFormatter('%g'+unit))

        fig.savefig(outfile, format='png')


if __name__ == "__main__":

    class ValidateMode(argparse.Action):
        def __call__(self, parser, args, values, option_string=None):
            valid_modes = ('sum', 'simple')
            mode = values[0]
            if mode not in valid_modes:
                raise ValueError('invalid mode %s' % subject)
            values = values[1:]
            setattr(args, self.dest, (mode, values))

    parser = argparse.ArgumentParser(
            description="Generate a graph from a benchmark output")

    parser.add_argument('-t','--title', help='title of the graph')
    parser.add_argument('-x','--xlabel', help='label of the x axis')
    parser.add_argument('-X','--xscale', default='linear',
            choices=['linear','log','loglog'],
            help='scale of the X axis (linear, log or loglog)')
    parser.add_argument('-Y','--yscale', default='linear',
            choices=['linear','log','loglog'],
            help='scale of the Y axis (linear, log or loglog)')
    parser.add_argument('-f','--polyfit',
            nargs='?', const='linear', default=None,
            choices=['linear','exp','log','loglog'],
            help='draw the best polynomial fit')
    parser.add_argument('-F','--fitdegree', default=1,
            type=int,help='degree of the polynomial fit')
    parser.add_argument('-e','--exitstatus', nargs='+',
            type=int, action='append', default=[],
            help='use datapoints with exit status N')

    parser.add_argument('-i', help='input file', nargs='?',
            type=argparse.FileType('r'),
            default=sys.stdin, dest='infile')
    parser.add_argument('-o', help='output file', nargs='?',
            type=argparse.FileType('wb'),
            default=sys.stdout, dest='outfile')

    parser.add_argument('-m','--mode', nargs='+', default=('sum',[]),
            action=ValidateMode, metavar=('MODE', 'ARGS'), help='TODO')

    args = parser.parse_args()

    args.modevars = args.mode[1][0].split() if len(args.mode[1]) == 1 \
                    else args.mode[1]
    args.mode = args.mode[0]
    args.exitstatus = [x for l in args.exitstatus for x in l] # Flatten

    g = Grapher()
    g.run(args.infile, args.outfile, args.mode, args.modevars, args.title,
            args.xlabel, args.xscale, args.yscale, args.polyfit, args.fitdegree,
            args.exitstatus)

