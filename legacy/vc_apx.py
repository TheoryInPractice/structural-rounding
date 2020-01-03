
"""
The vc_apx module implements algorithms for solving unweighted vertex cover approximately on simple graphs.

Methods
-------
dfs_apx(G)
	Solves vertex cover by constructing a DFS-tree.
std_apx(G)
	Solves vertex cover by constructing a maximal matching.
"""

import random
from collections import OrderedDict

def dfs_apx(G, subgraph=None):
	"""
	Solves vertex cover by constructing a DFS-tree.  The internal vertices make up the cover.  Achieves an approximation ratio of 2.

	Parameters
	----------
	G : Graph
		The graph to solve vertex cover on.  See graph.py.
	subgraph : set
		Optional.  A subset of the vertices to consider for the approximation.  If non-empty, finds an approximation on the subgraph induced by the vertices in the set.

	Returns
	-------
	set
		A subset of the vertices in G that make up a vertex cover.
	"""

	cover = set()
	stack = OrderedDict()

	# use the whole graph if subgraph not giver
	if not subgraph:
		subgraph = G.neighbors.keys()

	# randomly orders the vertices
	temp = random.sample(subgraph, len(subgraph))
	unvisited = OrderedDict()
	for t in temp:
		unvisited[t] = None

	while unvisited:
		# handles disconnected graphs
		if not stack:
			(current, nu) = unvisited.popitem()
		else:
			# child, parent
			(current, previous) = stack.popitem()
			unvisited.pop(current)

			# vertices are not added to the cover until they have a child
			# ensures only internal vertices are added
			cover.add(previous)

		# adds neighbors to the stack
		for nbr in G.neighbors[current]:
			if nbr in unvisited:
				if nbr in stack:
					# moves neighbor to the top of the stack if already present
					stack.move_to_end(nbr)
				stack[nbr] = current

	return cover

def std_apx(G, subgraph=None, rev=False):
	"""
	Solves vertex cover by constructing a maximal matching.  The vertices in the matching make up the cover.  Achieves an approximation ratio of 2.

	Parameters
	----------
	G : Graph
		The graph to solve vertex cover on.  See graph.py.
	subgraph : set
		Optional.  A subset of the vertices to consider for the approximation.  If non-empty, finds an approximation on the subgraph induced by the vertices in the set.
	rev : bool
		Reverses the order that vertices are sorted in so that low degree vertices chosen first.
	"""

	cover = set()

	# use whole graph if subgraph not specified
	if not subgraph:
		subgraph = G.neighbors.keys()

	# sorts vertices by degree
	# alternatively, vertices can be selected randomly
	temp = sorted(subgraph, key = lambda x: len(G.neighbors[x]), reverse=rev)
	# temp = random.sample(subgraph, len(subgraph))

	vertices = OrderedDict()
	for t in temp:
		vertices[t] = len(G.neighbors[t])

	while vertices:
		(u, nu) = vertices.popitem()
		v = None

		# chooses a random available neighbor of u to be matched to
		for nbr in G.neighbors[u]:
			if nbr in vertices:
				v = nbr
				break

		# skips u if there are no available neighbors
		if v == None:
			continue

		# adds both endpoints to the cover
		cover.add(u)
		cover.add(v)
		vertices.pop(v)

		# decreases degree of neighbors for potential removal
		for nbr in G.neighbors[u]:
			if nbr in vertices:
				vertices[nbr] -= 1
				if vertices[nbr] == 0:
					vertices.pop(nbr)

		for nbr in G.neighbors[v]:
			if nbr in vertices:
				vertices[nbr] -= 1
				if vertices[nbr] == 0:
					vertices.pop(nbr)

	return cover

def heuristic_apx(G):
	cover = set()

	deg = {}
	maxdeg = 0

	for v in G.neighbors.keys():
		deg[v] = len(G.neighbors[v])
		maxdeg = max(maxdeg, deg[v])

	revdeg = [None] * (maxdeg + 1)
	for i in range(maxdeg + 1):
		revdeg[i] = set()

	for v in deg.keys():
		revdeg[deg[v]].add(v)

	while deg.keys():
		for i in range(maxdeg, 0, -1):
			if revdeg[i]:
				maxdeg = i
				break

		u = random.sample(revdeg[maxdeg], 1)[0]
		revdeg[maxdeg].remove(u)
		deg.pop(u)
		cover.add(u)
		for v in G.neighbors[u]:
			if v not in deg:
				continue
				
			revdeg[deg[v]].remove(v)
			deg[v] -= 1
			if deg[v] == 0:
				deg.pop(v)
			else:
				revdeg[deg[v]].add(v)

	return cover
