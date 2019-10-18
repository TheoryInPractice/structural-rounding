
import cProfile

import sys, os, random
from time import time
from csv import DictWriter

from src.graph import Graph, read_edge_list, read_adj_list, read_sparse6
from src.octset import prescibed_octset, find_octset, verify_bip

from src.vc_apx import dfs_apx, std_apx, heuristic_apx
from src.vc_exact import bip_exact
from src.vc_lift import structural_rounding_apx, naive_lift, apx_lift, greedy_lift, oct_first_lift, bip_first_lift, recursive_lift, recursive_oct_lift, recursive_bip_lift

def run_apx(apx, graph, n):
    times = []
    sols = []
    for i in range(n):
        random.seed(i)
        start = time()
        cover = apx(graph)
        end = time()
        times.append(end - start)
        sols.append(len(cover))

    avgtime = round(sum(times) / n, 4)
    minsol = min(sols)
    maxsol = max(sols)
    return avgtime, minsol, maxsol

def run_lift(lift, graph, n, octset, partial):
    times = []
    sols = []
    for i in range(n):
        random.seed(i)
        start = time()
        cover, breakdown = lift(graph, octset, partial)
        end = time()
        times.append(end - start)
        sols.append(len(cover))

    avgtime = round(sum(times) / n, 4)
    minsol = min(sols)
    maxsol = max(sols)
    return avgtime, minsol, maxsol, breakdown

def main():
    filepath = sys.argv[1]
    n = 1

    with open("results/results.csv", "w") as f:
        header = ["name","n","m","dfs time","dfs size","heuristic time","heuristic size","std time","std size","stdrev time","stdrev size","oct size","partial","bip time","naive time","naive size","apx time","apx size","greedy time","greedy size","octfirst time","octfirst size","octfirst break","bipfirst time","bipfirst size","bipfirst break","rec time","rec size","rec break","recoct time","recoct size","recoct break","recbip time","recbip size","recbip break"]
        results = DictWriter(f, header)
        results.writeheader()

        for filename in os.listdir(filepath):
            if not filename.endswith(".s6"):
                continue

            res = {}

            graphname = filename.split(".s6")[0]
            print(graphname)
            res["name"] = graphname

            start = time()
            graph = read_sparse6("{}{}".format(filepath, filename))
            end = time()
            print("n: {}".format(len(graph.neighbors)))
            print("m: {}".format(graph.edges))
            print("time: {}".format(round(end - start, 4)))
            res["n"] = len(graph.neighbors)
            res["m"] = graph.edges

            t, minsol, maxsol = run_apx(heuristic_apx, graph, n)
            print("heuristic apx")
            print("\tavg time: {}".format(t))
            print("\tmin size: {}".format(minsol))
            print("\tmax size: {}".format(maxsol))
            res["heuristic time"] = t
            res["heuristic size"] = minsol

            # t, minsol, maxsol = run_apx(dfs_apx, graph, n)
            # print("dfs apx")
            # print("\tavg time: {}".format(t))
            # print("\tmin size: {}".format(minsol))
            # print("\tmax size: {}".format(maxsol))
            # res["dfs time"] = t
            # res["dfs size"] = minsol
            #
            # t, minsol, maxsol = run_apx(std_apx, graph, n)
            # print("std apx")
            # print("\tavg time: {}".format(t))
            # print("\tmin size: {}".format(minsol))
            # res["std time"] = t
            # res["std size"] = minsol
            #
            # alg = lambda x: std_apx(x, rev=True)
            # t, minsol, maxsol = run_apx(alg, graph, n)
            # print("\tmax size: {}".format(maxsol))
            # res["stdrev time"] = t
            # res["stdrev size"] = maxsol

            # octset, left, right = prescibed_octset(graph, "{}{}.oct".format(filepath, graphname))
            # octset, left, right = find_octset(graph)
            # octset, left, right = verify_bip(graph, set())
            # print(len(octset))

            # start = time()
            # partial = bip_exact(graph.vertex_subgraph(graph.neighbors.keys() - octset))
            # end = time()
            #
            # print("bip solve")
            # print("\tavg time: {}".format(round(end - start, 4)))
            #
            # res["oct size"] = len(octset)
            # res["partial"] = len(partial)
            # res["bip time"] = round(end - start, 4)
            #
            # t, minsol, maxsol, breakdown = run_lift(naive_lift, graph, n, octset, partial)
            # print("naive lift")
            # print("\tavg time: {}".format(t))
            # print("\tmin size: {}".format(minsol))
            # print("\tmax size: {}".format(maxsol))
            # res["naive time"] = t
            # res["naive size"] = minsol
            #
            # t, minsol, maxsol, breakdown = run_lift(apx_lift, graph, n, octset, partial)
            # print("apx lift")
            # print("\tavg time: {}".format(t))
            # print("\tmin size: {}".format(minsol))
            # print("\tmax size: {}".format(maxsol))
            # res["apx time"] = t
            # res["apx size"] = minsol
            #
            # t, minsol, maxsol, breakdown = run_lift(greedy_lift, graph, n, octset, partial)
            # print("greedy lift")
            # print("\tavg time: {}".format(t))
            # print("\tmin size: {}".format(minsol))
            # print("\tmax size: {}".format(maxsol))
            # res["greedy time"] = t
            # res["greedy size"] = minsol
            #
            # t, minsol, maxsol, breakdown = run_lift(oct_first_lift, graph, n, octset, partial)
            # print("octfirst lift")
            # print("\tavg time: {}".format(t))
            # print("\tmin size: {}".format(minsol))
            # print("\tmax size: {}".format(maxsol))
            # res["octfirst time"] = t
            # res["octfirst size"] = minsol
            # res["octfirst break"] = breakdown
            #
            # t, minsol, maxsol, breakdown = run_lift(bip_first_lift, graph, n, octset, partial)
            # print("bipfirst lift")
            # print("\tavg time: {}".format(t))
            # print("\tmin size: {}".format(minsol))
            # print("\tmax size: {}".format(maxsol))
            # res["bipfirst time"] = t
            # res["bipfirst size"] = minsol
            # res["bipfirst break"] = breakdown
            #
            # t, minsol, maxsol, breakdown = run_lift(recursive_lift, graph, n, octset, partial)
            # print("rec lift")
            # print("\tavg time: {}".format(t))
            # print("\tmin size: {}".format(minsol))
            # print("\tmax size: {}".format(maxsol))
            # res["rec time"] = t
            # res["rec size"] = minsol
            # res["rec break"] = breakdown
            #
            # t, minsol, maxsol, breakdown = run_lift(recursive_oct_lift, graph, n, octset, partial)
            # print("recoct lift")
            # print("\tavg time: {}".format(t))
            # print("\tmin size: {}".format(minsol))
            # print("\tmax size: {}".format(maxsol))
            # res["recoct time"] = t
            # res["recoct size"] = minsol
            # res["recoct break"] = breakdown
            #
            # t, minsol, maxsol, breakdown = run_lift(recursive_bip_lift, graph, n, octset, partial)
            # print("recbip lift")
            # print("\tavg time: {}".format(t))
            # print("\tmin size: {}".format(minsol))
            # print("\tmax size: {}".format(maxsol))
            # res["recbip time"] = t
            # res["recbip size"] = minsol
            # res["recbip break"] = breakdown

            results.writerow(res)
            del graph

            print()

if __name__ == "__main__":
    # cProfile.run("main()")
    main()
