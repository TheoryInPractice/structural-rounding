
from collections import defaultdict

class Graph:
    """
    Graph implements a basic data structure for simple, undirected, unweighted graphs (i.e. no loops or multi-edges).

    Attributes
    ----------
    neighbors : dict
        Map from vertices (int/str) to neighbors (set).
    edges : int
        The number of edges in the graph.

    Methods
    -------
    add_edge(u, v)
        Adds the edge uv to the graph by adding u to the set of v's neighbors and vice versa.
    vertex_subgraph(vertices)
        Returns a new graph containing the subgraph induced on the vertices in vertices.
    edge_subgraph(edges)
        Returns a new graph containing only the edges in edges and the vertices incident on those edges.
    """

    def __init__(self):
        """Initializes an empty graph with no edges or vertices."""

        # defaultdict chosen to speed up add_edge
        self.neighbors = defaultdict(lambda: set())
        self.edges = 0

    def add_edge(self, u, v):
        """
        Adds the edge uv to the graph by adding u to the set of v's neighbors and vice versa.

        Note that add_edge has no effect if uv is already an edge in the graph.

        Parameters
        ----------
        u : int/str
            One of the two vertices in the new edge.
        v : int/str
            One of the two vertices in the new edge.
        """

        # only counts the edge if not already present
        if u not in self.neighbors[v]:
            self.edges += 1

        self.neighbors[u].add(v)
        self.neighbors[v].add(u)

    def vertex_subgraph(self, vertices):
        """
        Returns a new graph containing the subgraph induced on the vertices in vertices.

        Vertices may contain vertices which are not in the original graph.  Vertices is assumed to be a set.

        Parameters
        ----------
        vertices : set
            The vertices from the graph to keep in the subgraph.

        Returns
        -------
        Graph
            A new Graph object representing the subgraph.
        """

        H = Graph()
        for u in vertices:
            # loops over neighbors which are also in the subgraph
            for v in self.neighbors[u] & vertices:
                H.add_edge(u, v)

        return H

    def edge_subgraph(self, edges):
        """
        Returns a new graph containing only the edges in edges and the vertices incident on those edges.

        Edges may contain edges (and incident vertices) which are not in the original graph.

        Parameters
        ----------
        edges : iterable
            The edges from the graph to keep in the subgraph.

        Returns
        -------
        Graph
            A new Graph object representing the subgraph.
        """

        H = Graph()
        for (u, v) in edges:
            # ensures that the edge was in the original graph
            if v in self.neighbors[u]:
                H.add_edge(u, v)

        return H

def read_edge_list(filename):
    """
    Creates a new Graph from an edge list stored in a file.

    The assumed file structure is one edge per line with the two vertices separated by whitespace.
    Assumes the names of the vertices are all ints.

    Parameters
    ----------
    filename : str
        The path/name of the file storing the graph.

    Returns
    -------
    Graph
        A new Graph object representing the graph from the file.
    """

    G = Graph()
    with open(filename, "r") as f:
        # reads each line of the file (1 edge per line)
        for line in f:
            edge = line.split()

            # converts to ints over strings for memory
            edge = (int(edge[0]), int(edge[1]))

            # discards self loops
            if edge[0] == edge[1]:
                continue

            G.add_edge(edge[0], edge[1])

    return G

def read_adj_list(filename):
    """
    Creates a new Graph from an adjacency list stored in a file.

    Assumes that the names of all of the vertices are ints.
    Expected file structure is one vertex per line with the vertex followed by all of its neighbors separated by whitespace.

    Parameters
    ----------
    filename : str
        The path/name of the file storing the graph.

    Returns
    -------
    Graph
        A new Graph object representing the graph from the file.
    """

    G = Graph()
    with open(filename, "r") as f:
        # reads each line of the file (1 vertex per line)
        for line in f:
            # removes newline and breaks up vertices into an array
            nbrs = line.strip().split()

            # each subsequent vertex is a neighbor of the first
            u = int(nbrs[0])
            for v in nbrs[1:]:
                v = int(v)

                # discards self loops
                if u == v:
                    continue

                G.add_edge(u, v)

    return G

def read_sparse6(filename):
    """
    Creates a new Graph from a sparse6 string stored in a file.

    Sparse6 format documented at [https://users.cecs.anu.edu.au/~bdm/data/formats.txt].
    Note that sparse6 typically allows for multiple graphs to be stored in one file, but this functionality is not implemented here.

    Parameters
    ----------
    filename : str
        The path/name of the file storing the graph.

    Returns
    -------
    Graph
        A new Graph object representing the graph from the file.
    """

    # Parses data (array of bytes) into b[i], x[i] stream
    # Each b[i] is 1 bit and each x[i] is k bits
    def parse(data, k):
        bitbuffer = 0
        available = 0

        while 1:
            try:
                if available < 1:
                    bitbuffer = next(data)
                    available = 6

                # retrieves one bit from buffer
                available -= 1
                b = bitbuffer >> available
                bitbuffer &= (1 << available) - 1

                # ensures there are k bits in the buffer
                while available < k:
                    bitbuffer = (bitbuffer << 6) + next(data)
                    available += 6

                # retrieves k bits from the buffer
                available -= k
                x = bitbuffer >> available
                bitbuffer &= (1 << available) - 1

                yield b, x

            # stops iterating when the stream is empty
            except StopIteration:
                break

    # reads the first line (sans newline) of the file as a byte array
    with open(filename, "rb") as f:
        string = f.readline().strip()

    # removes optional sparse6 header
    if string.startswith(b">>sparse6<<"):
        string = string[11:]

    # checks for required character in sparse6 format
    if not string.startswith(b":"):
        raise ValueError("Sparse6 format requires a \':\' at the beginning of the file.")

    # 63 is added in sparse6 format so each byte is a printable ascii character
    data = [c - 63 for c in string[1:]]

    # determines the number of vertices
    if data[0] <= 62:
        n = data[0]
        data = data[1:]
    elif data[1] <= 62:
        n = (data[1] << 12) + (data[2] << 6) + data[3]
        data = data[4:]
    else:
        n = (data[2] << 30) + (data[3] << 24) + (data[4] << 18) + (data[5] << 12) + (data[6] << 6) + data[7]
        data = data[8:]

    # log(n)
    k = 0
    while (1 << k) < n:
        k += 1

    G = Graph()
    data = iter(data)

    # reads stream based on sparse6 specification
    v = 0
    for b, x in parse(data, k):
        if b == 1:
            v += 1

        if x >= n or v >= n:
            break
        elif x > v:
            v = x
        else:
            G.add_edge(x, v)

    return G
