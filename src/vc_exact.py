
"""
The vc_exact module implements algorithms for solving unweighted vertex cover exactly on simple graphs.

Methods
-------
bip_exact(G)
    Solves vertex cover exactly on the bipartite graph G.
"""

from collections import deque, defaultdict

from .octset import verify_bip

def bip_exact(G):
    """
    Solves unwieghted vertex cover exactly on bipartite graphs.  Raises an error if the input is not bipartite.  Implements Hopcroft-Karp and Konig's Theorem.

    Parameters
    ----------
    G : Graph
        A bipartite graph.  See graph.py.

    Raises
    ------
    ValueError
        If G is not bipartite.

    Returns
    -------
    set
        A subset of the vertices in G that make up an optimal vertex cover.
    """

    # finds a bipartition of G
    # note octset is guaranteed to be empty if G is bipartite
    octset, left, right = verify_bip(G)

    # non-empty octset implies not bipartite
    if len(octset) > 0:
        raise ValueError("G is not bipartite")

    # keeps track of which vertices are paired in the matching
    match = {u:None for u in G.neighbors.keys()}

    # sub-algorithm for augmenting a matching (Hopcroft-Karp)
    # finds maximal set of disjoint shortest augmenting paths
    def augment():
        queue = deque()
        distance = defaultdict(lambda: -1)
        unmatched = set()

        # finds set of unmatched vertices on left
        for u in left:
            if match[u] == None:
                distance[u] = 0
                queue.append(u)
                unmatched.add(u)

        # performs bfs from unmatched vertices to find shortest augmenting paths
        while queue:
            current = queue.popleft()
            for nbr in G.neighbors[current]:
                # end of an augmenting path
                if match[nbr] == None:
                    continue

                if distance[match[nbr]] == -1:
                    distance[match[nbr]] = distance[current] + 1
                    queue.append(match[nbr])

        stack = []
        parent = {}
        update = False

        # performs dfs from unmatched vertices to extract disjoint shortest augmenting paths
        while unmatched or stack:
            if not stack:
                stack.append((unmatched.pop(), None))

            current, previous = stack.pop()
            parent[current] = previous
            for nbr in G.neighbors[current]:
                # if an augmenting path is found
                if match[nbr] == None:
                    update = True

                    # resets dfs to next unmatched vertex
                    stack = []
                    c = current
                    n = nbr

                    # toggles matches along the augmenting path
                    while c != None:
                        match[n] = c
                        temp = match[c]
                        match[c] = n
                        n = temp
                        c = parent[c]

                    break

                if distance[match[nbr]] == distance[current] + 1:
                    stack.append((match[nbr], current))

            distance[current] = -1

        return update
    # end of augment

    # finds a maximum matching by repeatedly augmenting
    cont = augment()
    while cont:
        cont = augment()

    queue = deque()
    visited = set()
    cover = set()

    for u in left:
        cover.add(u)
        if match[u] == None:
            visited.add(u)
            queue.append(u)

    # finds all vertices connected to unmatched by alternating paths
    # adds/removes from cover appropriately per Konig's
    while queue:
        current = queue.popleft()
        cover.remove(current)
        for nbr in G.neighbors[current]:
            if match[nbr] != current:
                cover.add(nbr)

            if match[nbr] == None:
                continue

            if match[nbr] not in visited:
                visited.add(match[nbr])
                queue.append(match[nbr])

    return cover
