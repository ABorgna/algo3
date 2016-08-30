#!/usr/bin/env python3

from pylab import log,loglog
import argparse
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
            xscale='linear', yscale='linear'):

        points = []

        for line in infile:
            values = [int(x) for x in line.split()]

            # Last number is the function return value
            # Ignore this datapoint if non zero
            ret = values.pop()
            if ret != 0:
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
            self.plotTime(points, outfile, title, xlabel, xscale, yscale)
        else:
            print("No datapoints available", file=sys.stderr)


    # Graphs

    def plotTime(self, points, outfile, title, xlabel, xscale, yscale):
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
        ax.set_title(title)
        ax.set_xlabel(xlabel)
        ax.set_ylabel("Tiempo")
        ax.set_xscale(xscale)
        ax.set_yscale(yscale)
        ax.set_ylim(ymin=0)
        ax.yaxis.set_major_formatter(mticker.FormatStrFormatter('%g'+unit))

        fig.savefig(outfile, format='png')


if __name__ == "__main__":
    parser = argparse.ArgumentParser(
            description="Generate a graph from a benchmark output")

    parser.add_argument('-t','--title', help='Title of the graph')
    parser.add_argument('-x','--xlabel', help='Label of the x axis')
    parser.add_argument('-X','--xscale', default='linear',
            choices=['linear','log','loglog'],
            help='Scale of the X axis (linear, log or loglog)')
    parser.add_argument('-Y','--yscale', default='linear',
            choices=['linear','log','loglog'],
            help='Scale of the Y axis (linear, log or loglog)')

    parser.add_argument('-i', help='Input file', nargs='?',
            type=argparse.FileType('r'),
            default=sys.stdin, dest='infile')
    parser.add_argument('-o', help='Output file', nargs='?',
            type=argparse.FileType('wb'),
            default=sys.stdout, dest='outfile')

    parser.add_argument('mode', nargs='?', default='sum',
            choices=['sum','simple'], help='sum or simple')
    parser.add_argument('modevars', metavar='var', nargs='*', help='')

    args = parser.parse_args()
    if(len(args.modevars) == 1):
        args.modevars = args.modevars[0].split()

    g = Grapher()
    g.run(args.infile, args.outfile, args.mode, args.modevars, args.title,
            args.xlabel, args.xscale, args.yscale)

