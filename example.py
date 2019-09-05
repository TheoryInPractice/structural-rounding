
"""
An example script that uses structual rounding to solve vertex cover.
"""

import sys
from time import time

from src.graph import Graph, read_edge_list, read_adj_list, read_sparse6
from src.octset import find_octset

from src.vc_apx import dfs_apx, std_apx
from src.vc_exact import bip_exact
from src.vc_lift import structural_rounding_apx, naive_lift, apx_lift, greedy_lift, oct_first_lift, bip_first_lift, recursive_lift, recursive_oct_lift, recursive_bip_lift

def main():
    # reads the graph in from an edge list in a file
    # also capable of reading adjacency list or sparse6
    filename = sys.argv[1]
    graph = read_edge_list(filename)

    # computes a 2-approximation using the maximal matching algorithm
    # also can use dfs-tree algorithm
    start = time()
    std_cover = std_apx(graph)
    end = time()
    print("std time: {}".format(round(end - start, 3)))
    print("std size: {}".format(len(std_cover)))

    # computes another solution using structural rounding with greedy lifting
    # several other lifts are available, see the import statements
    sr_alg = structural_rounding_apx(greedy_lift)
    start = time()
    sr_cover = sr_alg(graph)
    end = time()
    print("sr time: {}".format(round(end - start, 3)))
    print("sr size: {}".format(len(sr_cover)))

if __name__ == "__main__":
    main()
