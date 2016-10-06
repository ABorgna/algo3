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
        self.colors = {}

    def run(self, args):

        pointsSet = {}

        for line in args.infile:
            values = [int(x) for x in line.split()]

            # Last number is the function return value
            # Ignore this datapoint if non zero
            ret = values.pop()
            if len(args.exitstatus) and int(ret) not in args.exitstatus:
                continue

            # Next is the number of runs
            values.pop()

            # Minimum runtime, in nS
            time = values.pop()

            # What's left is the original variables
            x = None

            if args.mode == 'sum':
                x = sum(values)
            elif args.mode == 'prod':
                x = 1
                for v in values:
                    x *= v
            elif args.mode == 'simple':
                if len(values) != len(args.modevars):
                    print('Incorrect number of variables for mode "simple", expected',
                            len(values), 'but got', len(args.modevars), file=sys.stderr)
                    sys.exit(2)

                x = 0
                for mv, v in zip(args.modevars,values):
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

            if not args.colorize:
                ret = 0
            if not ret in pointsSet:
                pointsSet[ret] = []
            pointsSet[ret].append(point)

        for k in pointsSet.keys():
            pointsSet[k].sort()

        if len(pointsSet):
            self.plotTime(pointsSet, args)
        else:
            print("No datapoints available", file=sys.stderr)

    # Graphs

    def plotTime(self, pointsSet, args):

        maxY = max([points[-1][1] for points in pointsSet.values()])

        # The original times are in ns
        unit = "s"
        if maxY < 1e3:
            unit = "ns"
            scale = 1
        elif maxY < 1e6:
            unit = "us"
            scale = 1e3
        elif maxY < 1e9:
            unit = "ms"
            scale = 1e6
        elif maxY < 1e12:
            unit = "s"
            scale = 1e9

        pointsSet = {k: [(x,float(y)/scale) for x,y in points]
                        for k,points in pointsSet.items()}
        maxY = float(maxY)/scale
        graphScale = scale

        # Plot the data
        fig, ax = plt.subplots()
        fig.set_size_inches((12,9))

        for k, points in pointsSet.items():
            xs = [x for x,y in points]
            ys = [y for x,y in points]

            if k in args.statusname:
                label = args.statusname[k]
            else:
                label = "Hay solución" if k == 0 else "No hay solución"

            ax.plot(xs, ys, 'o', color=self.getColor(k),
                    label=label)

        if args.polyfit is not None and 0 in pointsSet:
            xs = [x for x,y in pointsSet[0]]
            ys = [y for x,y in pointsSet[0]]

            if args.polyfit == 'linear':
                scale = lambda y : y
                scaleback = lambda y : y
            elif args.polyfit == 'exp':
                scale = lambda y : log(y)
                scaleback = lambda y : math.e**y
            elif args.polyfit == 'log':
                scale = lambda y : math.e**y
                scaleback = lambda y : log(y)
            elif args.polyfit == 'loglog':
                scale = lambda y : math.e**(math.e**y)
                scaleback = lambda y : log(log(y))

            coef = np.polyfit(xs, [scale(y) for y in ys], args.fitdegree)

            fit = lambda x : scaleback(sum([a * x**(args.fitdegree-i)
                                 for (i,a) in enumerate(coef)]))
            ax.plot(xs, [fit(x) for x in xs], color=self.getColor("poly"),
                    linewidth=2)

        if len(args.line):
            xs = [x for points in pointsSet.values() for x,y in points]
            xs.sort()
            for (i,line) in enumerate(args.line):

                label = line[1] if len(line) > 1 else None

                # Fishy
                fn = eval('lambda x: '+line[0], math.__dict__)
                ax.plot(xs, [float(fn(x))/graphScale for x in xs],
                        color=self.COLORS[(4+i*4) % len(self.COLORS)],
                        linewidth=2, label=label)

        ax.set_title(args.title)
        ax.set_xlabel(args.xlabel)
        ax.set_ylabel("Tiempo")
        ax.set_xscale(args.xscale)
        ax.set_yscale(args.yscale)
        ax.set_ylim(ymin=0)

        if len(pointsSet) > 1 or len(args.line):
            ax.legend(loc='best')

        ax.yaxis.set_major_formatter(mticker.FormatStrFormatter('%g'+unit))

        fig.savefig(args.outfile, format='png')

    def getColor(self, id):
        if not id in self.colors:
            i = len(self.colors) * 2
            self.colors[id] = self.COLORS[i % len(self.COLORS)]
        return self.colors[id]

if __name__ == "__main__":

    class ValidateMode(argparse.Action):
        def __call__(self, parser, args, values, option_string=None):
            valid_modes = ('sum', 'prod', 'simple')
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
    parser.add_argument('-L','--line', type=str, nargs='+',
            default=[], metavar=('FN', 'LABEL'), action='append',
            help='draw a function of X written in python')
    parser.add_argument('-e','--exitstatus', nargs='+',
            type=int, action='append', default=[],
            help='only use datapoints with exit status N')
    parser.add_argument('-E','--statusname', nargs='+',
            type=str, action='append', default=[],
            help='label of each exitstatus selected via -e. Implies colorization')
    parser.add_argument('-C','--colorize', action='store_true', default=False,
            help='use diferent colors for each exitstatus')

    parser.add_argument('-i', help='input file', nargs='?',
            type=argparse.FileType('r'),
            default=sys.stdin, dest='infile')
    parser.add_argument('-o', help='output file', nargs='?',
            type=argparse.FileType('wb'),
            default=sys.stdout, dest='outfile')

    parser.add_argument('-m','--mode', nargs='+', default=('sum',[]),
            action=ValidateMode, metavar=('MODE', 'ARGS'),
            help='sum: use the sum of the variables as x value (default)\n' +
                 'prod: use the product of the variables as x value\n' +
                 'simple "* *>0 5 <10": use only the desired value for each variable')

    args = parser.parse_args()

    args.modevars = args.mode[1][0].split() if len(args.mode[1]) == 1 \
                    else args.mode[1]
    args.mode = args.mode[0]
    args.exitstatus = [x for l in args.exitstatus for x in l] # Flatten
    args.statusname = [x for l in args.statusname for x in l] # Flatten
    args.statusname = {x:s for (x,s) in zip(args.exitstatus,args.statusname)}
    if len(args.statusname):
        args.colorize = True

    g = Grapher()
    g.run(args)

