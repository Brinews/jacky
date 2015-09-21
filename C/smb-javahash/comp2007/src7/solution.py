import sys
import math

def read_board(input_file):
	""" Reads board from input_file and returns dict mapping non-crossed coordinates to vertex ids """
	board = set()
	for line in input_file.readlines():
		i, j = [ int(x) for x in line.split(",") ]
		board.add((i,j))

	return board


def reduction(board):
	""" Constructs the network flows instance (T,s,t,edges) for the given input board """

	# set target flow value
	T = int(math.ceil(len(board) / 2.0))
	
	# create vertex ids for non-crossed coordinates
	vertex_id = dict()
	for (i,j) in board:
		vertex_id[(i,j)] = len(vertex_id)

	# create two new ids for s and t
	s = len(vertex_id) + 1
	t = len(vertex_id) + 2

	# separate squares into white and black
	white = [ (i,j) for (i,j) in board if (i+j) % 2 == 0 ]
	black = [ (i,j) for (i,j) in board if (i+j) % 2 == 1 ]

	# create edge list
	edges = []

	# connect s to white squares
	for (i,j) in white:
		edges.append( (s, vertex_id[i,j], 1) )

	# connect black squares to t
	for (i,j) in black:
		edges.append( (vertex_id[i,j], t, 1) )

	# connect white to black squares
	for (i,j) in white:
		# list the potential neighbors of (i,j)
		potential_neighbors = [(i,j+1), (i+1, j), (i-1, j), (i, j-1)]
		for (a,b) in potential_neighbors:
			if (a,b) in board:
				edges.append([vertex_id[i,j], vertex_id[a,b],1])

	return (T, s, t, edges)


def print_instance(T,s,t,edges):
	""" Prints the network flows instance (T,s,t,edges) """
	print(T)
	print(s)
	print(t)
	for (u,v,c) in edges:
		print("{0},{1},{2}".format(u,v,c))



if __name__ == '__main__':
	board = read_board(sys.stdin)
	print_instance (*reduction(board))

