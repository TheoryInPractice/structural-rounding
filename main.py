

import cProfile

import sys, os, random
from time import time
from csv import DictWriter

from sr_apx.graph import Graph, read_edge_list, read_sparse6
from sr_apx.setmap import Set
from sr_apx.bipartite import prescribed_octset, vertex_delete, verify_bipartite

from sr_apx.vc.apx import dfs_apx, std_apx, heuristic_apx
from sr_apx.vc.exact import bip_exact
from sr_apx.vc.lift import naive_lift, greedy_lift, apx_lift, oct_lift, bip_lift, recursive_lift, recursive_oct_lift, recursive_bip_lift

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
        cover = lift(graph, octset, partial)
        end = time()
        times.append(end - start)
        sols.append(len(cover))

    avgtime = round(sum(times) / n, 4)
    minsol = min(sols)
    maxsol = max(sols)
    return avgtime, minsol, maxsol

def update_progress(completed, total):
    progress = int(completed / total * 100)
    progress_str = "#" * int(progress/2)
    progress_str += "-" * (50 - int(progress/2))
    print("[" + progress_str + "] {}%".format(progress), end="\r")

def main():
    filepath = sys.argv[1]
    if (filepath[len(filepath)-1] != '/'):
        filepath += "/";
    n = 1

    results_dir = os.path.join(os.getcwd(),"results")
    if not os.path.exists(results_dir):
        os.mkdir(results_dir)

    print("writing results to ./results/results.csv")

    graph_list = []
    for filename in os.listdir(filepath):
        if not filename.endswith(".s6"):
            continue

        graph_list.append(filename)

    with open("results/results.csv", "w") as f:
        header = ["name","n","m","dfs time","dfs size","heuristic time","heuristic size","std time","std size","stdrev time","stdrev size","oct size","partial","bip time","naive time","naive size","apx time","apx size","greedy time","greedy size","octfirst time","octfirst size","bipfirst time","bipfirst size","rec time","rec size","recoct time","recoct size","recbip time","recbip size"]
        results = DictWriter(f, header)
        results.writeheader()

        completed = 0
        total = len(graph_list) * 13
        for filename in graph_list:
            res = {}

            graphname = filename.split(".s6")[0]
            res["name"] = graphname

            graph = read_sparse6("{}{}".format(filepath, filename))
            res["n"] = len(graph)
            completed += 1
            update_progress(completed, total)

            t, minsol, maxsol = run_apx(heuristic_apx, graph, n)
            res["heuristic time"] = t
            res["heuristic size"] = minsol
            completed += 1
            update_progress(completed, total)

            t, minsol, maxsol = run_apx(dfs_apx, graph, n)
            res["dfs time"] = t
            res["dfs size"] = minsol
            completed += 1
            update_progress(completed, total)

            t, minsol, maxsol = run_apx(std_apx, graph, n)
            res["std time"] = t
            res["std size"] = minsol
            completed += 1
            update_progress(completed, total)

            # left, right, octset = prescibed_octset(graph, "{}{}.oct".format(filepath, graphname))

            start = time()
            octset = vertex_delete(graph)
            _, left, right = verify_bipartite(graph, octset)

            bippart = Set()
            for v in left:
                bippart.add(v)
            for v in right:
                bippart.add(v)


            partial = bip_exact(graph.subgraph(bippart))
            end = time()

            res["oct size"] = len(octset)
            res["partial"] = len(partial)
            res["bip time"] = round(end - start, 4)
            completed += 1
            update_progress(completed, total)

            t, minsol, maxsol = run_lift(naive_lift, graph, n, octset, partial)
            res["naive time"] = t
            res["naive size"] = minsol
            completed += 1
            update_progress(completed, total)

            t, minsol, maxsol = run_lift(greedy_lift, graph, n, octset, partial)
            res["greedy time"] = t
            res["greedy size"] = minsol
            completed += 1
            update_progress(completed, total)

            t, minsol, maxsol = run_lift(apx_lift, graph, n, octset, partial)
            res["apx time"] = t
            res["apx size"] = minsol
            completed += 1
            update_progress(completed, total)

            t, minsol, maxsol = run_lift(oct_lift, graph, n, octset, partial)
            res["octfirst time"] = t
            res["octfirst size"] = minsol
            completed += 1
            update_progress(completed, total)

            t, minsol, maxsol = run_lift(bip_lift, graph, n, octset, partial)
            res["bipfirst time"] = t
            res["bipfirst size"] = minsol
            completed += 1
            update_progress(completed, total)

            t, minsol, maxsol = run_lift(recursive_lift, graph, n, octset, partial)
            res["rec time"] = t
            res["rec size"] = minsol
            completed += 1
            update_progress(completed, total)

            t, minsol, maxsol = run_lift(recursive_oct_lift, graph, n, octset, partial)
            res["recoct time"] = t
            res["recoct size"] = minsol
            completed += 1
            update_progress(completed, total)

            t, minsol, maxsol = run_lift(recursive_bip_lift, graph, n, octset, partial)
            res["recbip time"] = t
            res["recbip size"] = minsol
            completed += 1
            update_progress(completed, total)

            results.writerow(res)
            del graph

    print()

if __name__ == "__main__":
    # cProfile.run("main()")
    main()
