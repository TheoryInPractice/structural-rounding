
"""
The octset module implements heuristic algorithms for finding odd-cycle transversals (deletion set to bipartite).

Methods
-------
prescibed_octset(G, filename)
    Reads a predetermined octset for the graph G in from a file.
verify_bip(G, octset)
    Uses a BFS-coloring to find on octset for the graph G.  The octset can be given to the algorithm to determine its validity.
find_octset(G)
    Finds an octset on G by producing two maximal independent sets to make up the bipartite portion of the graph.  The remaining vertices are returned as the octset.
"""

import random

from collections import deque, OrderedDict

def prescibed_octset(graph, filename):
    """
    Reads a predetermined octset from a file and verifies its correctness.

    Parameters
    ----------
    graph : Graph
        The graph corresponding to the octset.  See graph.py.
    filename : str
        Name of the file containing the octset.

    Returns
    -------
    (set, set, set)
        A tuple of sets of size three corresponding to the octset, the left partition, and the right partition respectively.
    """

    octset = set()

    # opens the file
    with open(filename, "r") as f:
        for line in f:
            octset.add(int(line))

    # verifies that removing the octset makes the graph bipartite
    nu, left, right = verify_bip(graph, octset)
    return octset, left, right

def verify_bip(G, octset=set()):
    """
    Finds an octset on G by doing BFS.  When a vertex is added to both the left and the right, it gets added to the octset.

    Parameters
    ----------
    G : Graph
        The graph to solve oct on.  See graph.py.
    octset : set
        Optional.  Seeds the octset with vertices to add correctness-checking functionality.

    Returns
    -------
    (set, set, set)
        A tuple of sets of size three corresponding to the octset, left partition, and right partition respectively.
    """

    # sets up data structures for bfs
    queue = deque()
    unvisited = set(G.neighbors.keys() - octset)

    left = set()
    right = set()

    while unvisited or queue:
        # handles disconnected case
        if not queue:
            current = unvisited.pop()

            # adds first vertex to the left
            left.add(current)
        else:
            current = queue.popleft()

        for nbr in G.neighbors[current]:
            if nbr in octset:
                continue

            # adds each neighbor to the opposite side
            side = right if current in left else left
            side.add(nbr)

            # if the neighbor is now on both sides, must go in oct
            if nbr in left and nbr in right:
                left.remove(nbr)
                right.remove(nbr)
                octset.add(nbr)

            # add neighbor to queue if unseen, bfs
            if nbr in unvisited:
                unvisited.remove(nbr)
                queue.append(nbr)

    return octset, left, right

def find_octset(G):
    """
    Finds an octset on G by constructing two maximal independent sets in sequence.  Any vertices not in either independent set are added to the octset.

    Note that two implementations of maximal_indset are given.  The first (commented out) does not perform resorting based on degree and thus runs faster but produces larger octsets as a result.

    Parameters
    ----------
    G : Graph
        The graph to solve oct on.  See graph.py.

    Returns
    -------
    (set, set, set)
        A tuple of sets of size three corresponding to the octset, left partition, and right partition respectively.
    """

    # def maximal_indset(G, allowed):
    #     # choice between random selection and favoring small degree vertices
    #     # temp = sorted(allowed, key = lambda x: len(G.neighbors[x]))
    #     temp = random.sample(allowed, len(allowed))
    #
    #     # copy vertices to ordereddict for efficiency
    #     allowed = OrderedDict()
    #     for v in temp:
    #         allowed[v] = None
    #
    #     indset = set()
    #
    #     while allowed:
    #         # adds a vertex to the independent set
    #         (current, nu) = allowed.popitem()
    #         indset.add(current)
    #
    #         # removes all neighbors so they can't be added to the independent set
    #         for v in G.neighbors[current]:
    #             if v in allowed:
    #                 allowed.pop(v)
    #
    #     return indset

    def maximal_indset(G, allowed):
        deg = {}
        maxdeg = 0
        mindeg = len(allowed)

        # computes the degree of each allowed vertex
        for v in G.neighbors.keys():
            if v not in allowed:
                continue

            deg[v] = 0
            for u in G.neighbors[v]:
                if u in allowed:
                    deg[v] += 1

            maxdeg = max(maxdeg, deg[v])
            mindeg = min(mindeg, deg[v])

        # creates a map of degrees to sets of vertices with that degree
        revdeg = [None] * (maxdeg + 1)
        for i in range(maxdeg + 1):
            revdeg[i] = set()

        for v in deg.keys():
            revdeg[deg[v]].add(v)

        indset = set()

        while deg.keys():
            # finds the lowest degree vertex
            for i in range(mindeg, maxdeg + 1):
                if revdeg[i]:
                    mindeg = i
                    break

            # randomly picks a lowest degree vertex
            u = random.sample(revdeg[mindeg], 1)[0]
            revdeg[mindeg].remove(u)
            mindeg = max(mindeg - deg[u], 0)
            deg.pop(u)
            for v in G.neighbors[u]:
                if v not in deg:
                    continue

                # removes neighbors
                revdeg[deg[v]].remove(v)
                deg.pop(v)

                # updates degrees
                for w in G.neighbors[v]:
                    if w in deg:
                        revdeg[deg[w]].remove(w)
                        deg[w] -= 1
                        revdeg[deg[w]].add(w)

            indset.add(u)

        return indset

    # octset begins as entire graph
    octset = set(G.neighbors.keys())
    left = maximal_indset(G, octset)

    # remove vertices which are in the left partition
    octset.difference_update(left)
    right = maximal_indset(G, octset)

    # remove vertices which are in the right partition
    octset.difference_update(right)
    return octset, left, right
