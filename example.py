
"""
An example script that uses structual rounding to solve vertex cover.
"""

import sys
from time import time

from sr_apx.graph import Graph, read_edge_list, read_sparse6
from sr_apx.setmap import Set
from sr_apx.octset import find_octset

from sr_apx.vc.apx import dfs_apx, std_apx, heuristic_apx
from sr_apx.vc.exact import bip_exact
from sr_apx.vc.lift import naive_lift, greedy_lift

def main():
    # reads the graph in from an edge list in a file
    # also capable of reading adjacency list or sparse6
    filename = sys.argv[1]
    graph = read_sparse6(filename)

    # computes a 2-approximation using the maximal matching algorithm
    # also can use dfs-tree algorithm
    start = time()
    std_cover = std_apx(graph)
    end = time()
    print("std time: {}".format(round(end - start, 3)))
    print("std size: {}".format(len(std_cover)))

    # computes another solution using structural rounding with greedy lifting
    # several other lifts are available, see the import statements
    start = time()
    left, right, octset = find_octset(graph)

    bippart = Set()
    for v in left:
        bippart.add(v)
    for v in right:
        bippart.add(v)

    subgraph = graph.subgraph(bippart)
    partial = bip_exact(subgraph)
    sr_cover = greedy_lift(graph, octset, partial)
    end = time()
    print("sr time: {}".format(round(end - start, 3)))
    print("sr size: {}".format(len(sr_cover)))

if __name__ == "__main__":
    main()
