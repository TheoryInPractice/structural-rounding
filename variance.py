
import sys, os, random
from time import time
from csv import DictWriter

from sr_apx.setmap import Set
from sr_apx.graph import Graph, read_sparse6

from sr_apx.bipartite import vertex_delete, verify_bipartite

from sr_apx.vc.exact import bip_exact
from sr_apx.vc.apx import dfs_apx, std_apx
from sr_apx.vc.lift import naive_lift, greedy_lift

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
            print("n: {}".format(len(graph)))
            res["n"] = len(graph)

            mean, var = runtime(vertex_delete, graph, n)
            print("oct time")
            print("\tmean: {}".format(mean))
            print("\tvar: {}".format(var))
            res["octtime mean"] = mean
            res["octtime var"] = var

            o = vertex_delete(graph)
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

            alg = lambda x: verify_bipartite(x, Set())
            mean, var = runtime(alg, graph, n)
            print("bfs time")
            print("\tmean: {}".format(mean))
            print("\tvar: {}".format(var))
            res["bfstime mean"] = mean
            res["bfstime var"] = var

            o, l, r = verify_bipartite(graph, Set())
            res["bfssize mean"] = len(o)
            print("bfs size")
            print("\tmean: {}".format(len(o)))

            octset = vertex_delete(graph)
            _, left, right = verify_bipartite(graph, octset)
            bippart = Set()
            for v in left:
                bippart.add(v)
            for v in right:
                bippart.add(v)
            g = graph.subgraph(bippart)

            mean, var = runtime(bip_exact, g, n)
            print("bip time")
            print("\tmean: {}".format(mean))
            print("\tvar: {}".format(var))
            res["biptime mean"] = mean
            res["biptime var"] = var

            partial = bip_exact(g)

            alg = lambda x: naive_lift(x, octset, partial)
            mean, var = runtime(alg, graph, n)
            print("naive time")
            print("\tmean: {}".format(mean))
            print("\tvar: {}".format(var))
            res["naivetime mean"] = mean
            res["naivetime var"] = var

            alg = lambda x: greedy_lift(x, octset, partial)
            mean, var = runtime(alg, graph, n)
            print("greedy time")
            print("\tmean: {}".format(mean))
            print("\tvar: {}".format(var))
            res["greedytime mean"] = mean
            res["greedytime var"] = var

            results.writerow(res)
            print()
