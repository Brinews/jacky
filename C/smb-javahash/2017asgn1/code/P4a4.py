import networkx as nx

g = nx.DiGraph()

#################################################

k, Y = int(input()), int(input())
#n, m = int(input()), int(input())

# read in theta
theta = []
for i in range(Y):
    theta.append(int(input()))

# read in Ti
T = []
for i in range(k):
    T.append(int(input()))

W = []
# read in W
for i in range(k):
    Wi = []
    for j in range(Y):
        Wi.append(int(input()))

    W.append(Wi)

u = []
# read in u
for i in range(Y):
    u.append(int(input()))

#################################################

# add start node
g.add_node(0)

# add year nodes to net
for i in range(Y):
    g.add_node(i+1)

# add each forest of different year nodes to net
for i in range(Y):
    for j in range(k):
        g.add_node(Y+i*k+j+1)

# add forest collect nodes to net
for i in range(k):
    g.add_node(Y+k*Y+i+1)

# add end node
cur = Y+k+k*Y
g.add_node(cur+1)


#################################################

# add start <- year node edges to net
for i in range(Y):
    a = 0
    b = i+1
    c = u[i]
    g.add_edge(b, a, capacity=c)

# add year <- forest edges to net
for i in range(Y):
    for j in range(k):
        a = i+1
        b = Y+i*k+j+1
        c = W[j][i]
        g.add_edge(b, a, capacity=c)


# add forest <- forest edges to net
for i in range(Y-1):
    m = theta[i]
    for j in range(k):
        a = Y+i*k+j+1
        #b = Y+(i+1)*k+j+1
        b = 1+i+1
        c = W[j][i]
        n = 1
        while n < m:
            #print("%d-%d-%d" % (b, a, c))
            g.add_edge(a, b, capacity=c)
            n = n+1
            b = b+1


# add forest <- forest-collect edges to net
for i in range(Y):
    m = theta[i]
    for j in range(k):
        a = Y+i*k+j+1
        b = Y+k*Y+j+1
        c = W[j][i]
        if m == 0:
            c = 0
        g.add_edge(b, a, capacity=c)

# add forest-collect <- end edges to net
for i in range(k):
    a = Y+k*Y+i+1
    b = Y+k+k*Y+1
    c = T[i]
    g.add_edge(b, a, capacity=c)

	
#################################################
max_flow = nx.algorithms.flow.maxflow.maximum_flow(g, Y+k+k*Y+1, 0)[0]
print(max_flow)
