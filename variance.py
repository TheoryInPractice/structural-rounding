
import sys, os, random
from time import time
from csv import DictWriter

from src.graph import read_sparse6
from src.octset import find_octset, verify_bip

from src.vc_exact import bip_exact
from src.vc_apx import dfs_apx, std_apx
from src.vc_lift import naive_lift, apx_lift, greedy_lift, oct_first_lift, bip_first_lift, recursive_lift, recursive_oct_lift, recursive_bip_lift

def runtime(alg, G, n):
    times = []
    for i in range(n):
        random.seed(i)
        start = time()
        alg(G)
        end = time()
        times.append(end - start)

    mean = sum(times) / n
    var = 0
    for t in times:
        var += (t - mean) ** 2

    var /= n
    return mean, var

def solsize(alg, G, n1, n2):
    sols = []
    for i in range(n1):
        inner = []
        for j in range(n2):
            random.seed(i * n1 + j)
            s = alg(G)
            inner.append(len(s))

        sols.append(min(inner))

    mean = sum(sols) / n1
    var = 0
    for s in sols:
        var += (s - mean) ** 2

    var /= n1
    return mean, var

if __name__ == "__main__":
    filepath = sys.argv[1]
    n = 50

    with open("results/variance.csv", "w") as f:
        header = ["name", "octtime mean", "octtime var", "dfstime mean", "dfstime var", "stdtime mean", "stdtime var", "biptime mean", "biptime var", "naivetime mean", "naivetime var", "apxtime mean", "apxtime var", "greedytime mean", "greedytime var", "octfirsttime mean", "octfirsttime var", "bipfirsttime mean", "bipfirsttime var", "rectime mean", "rectime var", "recocttime mean", "recocttime var", "recbiptime mean", "recbiptime var", "dfssize mean", "dfssize var", "stdsize mean", "stdsize var", "octsize mean", "octsize var", "n", "bfssize mean", "bfstime mean", "bfstime var"]
        results = DictWriter(f, header)
        results.writeheader()

        for filename in os.listdir(filepath):
            if not filename.endswith(".s6"):
                continue

            res = {}

            graphname = filename.split(".s6")[0]
            print(graphname)
            res["name"] = graphname

            graph = read_sparse6("{}{}".format(filepath, filename))
            print("n: {}".format(len(graph.neighbors)))
            print("m: {}".format(graph.edges))
            res["n"] = len(graph.neighbors)

            mean, var = runtime(find_octset, graph, n)
            print("oct time")
            print("\tmean: {}".format(mean))
            print("\tvar: {}".format(var))
            res["octtime mean"] = mean
            res["octtime var"] = var

            o, l, r = find_octset(graph)
            res["octsize mean"] = len(o)
            print("oct size")
            print("\tmean: {}".format(len(o)))

            mean, var = runtime(dfs_apx, graph, n)
            print("dfs time")
            print("\tmean: {}".format(mean))
            print("\tvar: {}".format(var))
            res["dfstime mean"] = mean
            res["dfstime var"] = var

            mean, var = solsize(dfs_apx, graph, n, 1)
            print("dfs size")
            print("\tmean: {}".format(mean))
            print("\tvar: {}".format(var))
            res["dfssize mean"] = mean
            res["dfssize var"] = var

            mean, var = runtime(std_apx, graph, n)
            print("std time")
            print("\tmean: {}".format(mean))
            print("\tvar: {}".format(var))
            res["stdtime mean"] = mean
            res["stdtime var"] = var

            mean, var = solsize(std_apx, graph, n, 1)
            print("std size")
            print("\tmean: {}".format(mean))
            print("\tvar: {}".format(var))
            res["stdsize mean"] = mean
            res["stdsize var"] = var

            mean, var = runtime(verify_bip, graph, n)
            print("bfs time")
            print("\tmean: {}".format(mean))
            print("\tvar: {}".format(var))
            res["bfstime mean"] = mean
            res["bfstime var"] = var

            o, l, r = verify_bip(graph, set())
            res["bfssize mean"] = len(o)
            print("bfs size")
            print("\tmean: {}".format(len(o)))

            octset, left, right = find_octset(graph)
            g = graph.vertex_subgraph(graph.neighbors.keys() - octset)

            mean, var = runtime(bip_exact, g, n)
            print("bip time")
            print("\tmean: {}".format(mean))
            print("\tvar: {}".format(var))
            res["biptime mean"] = mean
            res["biptime var"] = var

            partial = bip_exact(g)

            alg = lambda x: naive_lift(x, octset, partial)[0]
            mean, var = runtime(alg, graph, n)
            print("naive time")
            print("\tmean: {}".format(mean))
            print("\tvar: {}".format(var))
            res["naivetime mean"] = mean
            res["naivetime var"] = var

            alg = lambda x: apx_lift(x, octset, partial)[0]
            mean, var = runtime(alg, graph, n)
            print("apx time")
            print("\tmean: {}".format(mean))
            print("\tvar: {}".format(var))
            res["apxtime mean"] = mean
            res["apxtime var"] = var

            alg = lambda x: greedy_lift(x, octset, partial)[0]
            mean, var = runtime(alg, graph, n)
            print("greedy time")
            print("\tmean: {}".format(mean))
            print("\tvar: {}".format(var))
            res["greedytime mean"] = mean
            res["greedytime var"] = var

            alg = lambda x: oct_first_lift(x, octset, partial)[0]
            mean, var = runtime(alg, graph, n)
            print("octfirst time")
            print("\tmean: {}".format(mean))
            print("\tvar: {}".format(var))
            res["octfirsttime mean"] = mean
            res["octfirsttime var"] = var

            alg = lambda x: bip_first_lift(x, octset, partial)[0]
            mean, var = runtime(alg, graph, n)
            print("bipfirst time")
            print("\tmean: {}".format(mean))
            print("\tvar: {}".format(var))
            res["bipfirsttime mean"] = mean
            res["bipfirsttime var"] = var

            alg = lambda x: recursive_lift(x, octset, partial)[0]
            mean, var = runtime(alg, graph, n)
            print("rec time")
            print("\tmean: {}".format(mean))
            print("\tvar: {}".format(var))
            res["rectime mean"] = mean
            res["rectime var"] = var

            alg = lambda x: recursive_oct_lift(x, octset, partial)[0]
            mean, var = runtime(alg, graph, n)
            print("recoct time")
            print("\tmean: {}".format(mean))
            print("\tvar: {}".format(var))
            res["recocttime mean"] = mean
            res["recocttime var"] = var

            alg = lambda x: recursive_bip_lift(x, octset, partial)[0]
            mean, var = runtime(alg, graph, n)
            print("recbip time")
            print("\tmean: {}".format(mean))
            print("\tvar: {}".format(var))
            res["recbiptime mean"] = mean
            res["recbiptime var"] = var

            results.writerow(res)
            print()
