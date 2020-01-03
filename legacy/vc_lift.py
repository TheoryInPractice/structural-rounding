
"""
The vc_lift module provides implementations of various lifting strategies for vertex cover on near-bipartite graphs to be used in a structural rounding algorithm.

Methods
-------
structural_rounding_apx(lift)
    Creates a function to solve vertex cover using structural rounding..
naive_lift(G, octset, partial)
    Finds a vertex cover on G by adding every vertex in the octset to the partial solution.
greedy_lift(G, octset, partial)
    Finds a vertex cover on G by greedily adding necessary vertices from the octset to the partial solution.
apx_lift(G, octset, partial)
    Finds a vertex cover on G by approximating a vertex cover on G - partial.
oct_first_lift(G, octset, partial)
    Finds a vertex cover on G by approximating a solution on the octset and then exactly solving vertex cover on the bipartite portion remaining.
bip_first_lift(G, octset, partial)
    Finds a vertex cover on G by solving the bipartite portion first (see oct_first_lift).
recursive_lift(G, octset, partial)
    Similar to apx_lift.  Uses structural rounding instead of std_apx.
recursive_oct_lift(G, octset, partial)
    Similar to oct_first_lift.  Uses structural rounding instead of std_apx.
recursive_bip_lift(G, octset, partial)
    Similar to bip_first_lift.  Uses structural rounding instead of std_apx.
"""

from .octset import find_octset
from .vc_exact import bip_exact
from .vc_apx import dfs_apx, std_apx

def structural_rounding_apx(lift):
    """
    Creates a function to solve vertex cover using structural rounding with the given lifting strategy.

    Parameters
    ----------
    lift : function
        The lifting strategy to use in the structural rounding algorithm.

    Returns
    -------
    function
        A callable function to run structural rounding on an input graph.
    """

    def algorithm(G):
        octset, left, right = find_octset(G)
        bip_part = G.vertex_subgraph(G.neighbors.keys() - octset)
        partial = bip_exact(bip_part)
        cover, breakdown = lift(G, octset, partial)
        return cover

    return algorithm

def naive_lift(G, octset, partial):
    """
    Finds a vertex cover on G by adding every vertex in the octset to the partial solution.

    Parameters
    ----------
    G : Graph
        The input graph.  See graph.py.
    octset : set
        A set of vertices in G whose deletion makes G bipartite.
    partial : set
        A set of vertices in G that form a vertex cover on G - octset.

    Returns
    -------
    set, tuple
        The vertex cover and how many vertices were added by the lift.
    """

    return octset | partial, (len(octset))

def greedy_lift(G, octset, partial):
    """
    Finds a vertex cover on G by greddily adding necessary vertices in the octset to the partial solution.

    Parameters
    ----------
    G : Graph
        The input graph.  See graph.py.
    octset : set
        A set of vertices in G whose deletion makes G bipartite.
    partial : set
        A set of vertices in G that form a vertex cover on G - octset.

    Returns
    -------
    set, tuple
        The vertex cover and how many vertices were added by the lift.
    """

    nonoct = G.neighbors.keys() - octset
    cover = set(partial)
    for v in octset:
        for nbr in G.neighbors[v]:
            # if a neighbor of v that has already been seen is not in the octset
            if nbr in nonoct and nbr not in cover:
                cover.add(v)
                break

        nonoct.add(v)

    return cover, (len(cover) - len(partial))

def apx_lift(G, octset, partial):
    """
    Finds a vertex cover on G by approximating a vertex cover on G - partial.

    Parameters
    ----------
    G : Graph
        The input graph.  See graph.py.
    octset : set
        A set of vertices in G whose deletion makes G bipartite.
    partial : set
        A set of vertices in G that form a vertex cover on G - octset.

    Returns
    -------
    set, tuple
        The vertex cover and how many vertices were added by the lift.
    """

    cover = std_apx(G, G.neighbors.keys() - partial)
    cover.update(partial)
    return cover, (len(cover) - len(partial))

def oct_first_lift(G, octset, partial):
    """
    Finds a vertex cover on G by approximating a solution on the octset and then exactly solving vertex cover on the bipartite portion remaining.

    Parameters
    ----------
    G : Graph
        The input graph.  See graph.py.
    octset : set
        A set of vertices in G whose deletion makes G bipartite.
    partial : set
        A set of vertices in G that form a vertex cover on G - octset.

    Returns
    -------
    set, tuple
        The vertex cover and how many vertices were added by the lift.
    """

    # approximates a vertex cover on the octset
    cover = std_apx(G, octset)
    size1 = len(cover)
    cover.update(partial)

    # removes all verrtices that have already been added to the cover
    # note this leaves a bipartite graph
    H = G.vertex_subgraph(G.neighbors.keys() - cover)
    cover.update(bip_exact(H))

    return cover, (len(cover) - len(partial) - size1, size1)

def bip_first_lift(G, octset, partial):
    """
    Finds a vertex cover on G by solving the bipartite portion first (see oct_first_lift).

    Parameters
    ----------
    G : Graph
        The input graph.  See graph.py.
    octset : set
        A set of vertices in G whose deletion makes G bipartite.
    partial : set
        A set of vertices in G that form a vertex cover on G - octset.

    Returns
    -------
    set, tuple
        The vertex cover and how many vertices were added by the lift.
    """

    # generator to produce edges between the octset and the remainder of the graph
    def gen():
        for u in G.neighbors:
            if u not in octset:
                continue

            for v in G.neighbors[u]:
                if v not in octset and v not in partial:
                    yield (u, v)

    # computes an exact vertex cover for the edges between the octset and the bipartite portion
    H = G.edge_subgraph(gen())
    cover = bip_exact(H)
    size1 = len(cover)

    # approximates a vertex cover for the remaining part of the graph
    cover.update(std_apx(G, octset - cover))
    cover.update(partial)
    return cover, (size1, len(cover) - len(partial) - size1)

def recursive_lift(G, octset, partial):
    """
    Similar to apx_lift.  Uses structural rounding instead of std_apx.

    Parameters
    ----------
    G : Graph
        The input graph.  See graph.py.
    octset : set
        A set of vertices in G whose deletion makes G bipartite.
    partial : set
        A set of vertices in G that form a vertex cover on G - octset.

    Returns
    -------
    set, tuple
        The vertex cover and how many vertices were added by the lift.
    """

    # removes the vertices already in the octset
    H = G.vertex_subgraph(G.neighbors.keys() - partial)

    # structural rounding with naive lift on H
    oct2, left, right = find_octset(H)
    I = H.vertex_subgraph(H.neighbors.keys() - oct2)
    cover = bip_exact(I)
    cover.update(oct2)
    cover.update(partial)

    return cover, (len(cover) - len(partial) - len(oct2), len(oct2))

def recursive_oct_lift(G, octset, partial):
    """
    Similar to oct_first_lift.  Uses structural rounding instead of std_apx.

    Parameters
    ----------
    G : Graph
        The input graph.  See graph.py.
    octset : set
        A set of vertices in G whose deletion makes G bipartite.
    partial : set
        A set of vertices in G that form a vertex cover on G - octset.

    Returns
    -------
    set, tuple
        The vertex cover and how many vertices were added by the lift.
    """

    # subgraph with just the oct vertices
    H = G.vertex_subgraph(octset)

    # structural rounding with naive lifting on H
    oct2, left, right = find_octset(H)
    I = H.vertex_subgraph(H.neighbors.keys() - oct2)
    cover = bip_exact(I)
    size1 = len(cover)
    cover.update(oct2)
    cover.update(partial)

    # exact vertex cover on the remaining bipartite set of edges
    H = G.vertex_subgraph(G.neighbors.keys() - cover)
    cover.update(bip_exact(H))
    return cover, (len(cover) - len(partial) - len(oct2) - size1, size1, len(oct2))

def recursive_bip_lift(G, octset, partial):
    """
    Similar to bip_first_lift.  Uses structural rounding instead of std_apx.

    Parameters
    ----------
    G : Graph
        The input graph.  See graph.py.
    octset : set
        A set of vertices in G whose deletion makes G bipartite.
    partial : set
        A set of vertices in G that form a vertex cover on G - octset.

    Returns
    -------
    set, tuple
        The vertex cover and how many vertices were added by the lift.
    """

    # generator for edges between the octset and the rest of the graph
    def gen():
        for u in G.neighbors:
            if u not in octset:
                continue

            for v in G.neighbors[u]:
                if v not in octset and v not in partial:
                    yield (u, v)

    # bipartite subgraph between octset and rest of graph
    H = G.edge_subgraph(gen())
    cover = bip_exact(H)
    size1 = len(cover)

    H = G.vertex_subgraph(octset - cover)

    # structural rounding with naive lifting on remainder of graph
    oct2, left, right = find_octset(H)
    I = H.vertex_subgraph(H.neighbors.keys() - oct2)
    cover.update(bip_exact(I))
    size2 = len(cover) - size1
    cover.update(oct2)
    cover.update(partial)

    return cover, (size1, size2, len(oct2))
