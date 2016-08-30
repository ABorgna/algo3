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

    def run(self, infile, outfile, mode, modevars, title='', xlabel='', log=False):

        points = []

        for line in infile:
            values = [int(x) for x in line.split()]

            # Last number is the function return value
            values.pop()

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

                for mv, v in zip(modevars,values):
                    if mv == 'x':
                        x = v
                    elif mv != '*':
                        # mv is a number, add this point only if v coincides
                        if int(mv) != v:
                            x = None
                            break

            if x is None:
                break

            point = (x,time)
            points.append(point)

        points.sort()

        self.plotTime(points, outfile, title, xlabel, log)


    # Graphs

    def plotTime(self, points, outfile, title, xlabel, log):
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

        ax.plot(xs,ys, color=self.COLORS[0])
        ax.set_title(title)
        ax.set_xlabel(xlabel)
        ax.set_ylabel("Tiempo")
        ax.yaxis.set_major_formatter(mticker.FormatStrFormatter('%g'+unit))

        if log:
            ax.set_xscale('log')

        fig.savefig(outfile, format='png')


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Generate a graph from a benchmark output")

    parser.add_argument('-l','--log', action='store_true', help='Logarithmic scale')
    parser.add_argument('-t','--title', help='Title of the graph')
    parser.add_argument('-x','--xlabel', help='Label of the x axis')

    parser.add_argument('-i', help='Input file', nargs='?', type=argparse.FileType('r'),
                        default=sys.stdin, dest='infile')
    parser.add_argument('-o', help='Output file', nargs='?', type=argparse.FileType('wb'),
                        default=sys.stdout, dest='outfile')

    parser.add_argument('mode', nargs='?', default='sum', choices=['sum','simple'],
                        help='sum or simple')
    parser.add_argument('modevars', metavar='var', nargs='*',
                        help='')

    args = parser.parse_args()
    if(len(args.modevars) == 1):
        args.modevars = args.modevars[0].split()

    g = Grapher()
    g.run(args.infile, args.outfile, args.mode, args.modevars, args.title, args.xlabel, args.log)

