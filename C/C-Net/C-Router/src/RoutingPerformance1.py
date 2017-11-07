#!/usr/bin/env python
#coding:utf-8

import sys
import queue
import random

# for debug
def printTop(topdict):
    # print top
    for k in sorted(topdict.keys()):
        print(k)
        for m in sorted(topdict[k].keys()):
            print(m, topdict[k][m].getProbagation())

class Edge:
    def __init__(self):
        self.runTimeLink = 0

    def initLink(self, src, dst, pro, maxc):
        self.V1 = src
        self.V2 = dst
        self.probagation = pro
        self.maxlink = maxc

    def initDie(self, src, dst, die):
        self.V1 = src
        self.V2 = dst
        self.dieTime = die

    def getVert1(self):
        return self.V1

    def getVert2(self):
        return self.V2

    def getProbagation(self):
        return self.probagation

    def getRunTimeLink(self):
        return self.runTimeLink

    def decreaseRunTimeLink(self):
        self.runTimeLink = self.runTimeLink - 1

    def increaseRunTimeLink(self):
        self.runTimeLink = self.runTimeLink + 1

    def getMaxLink(self):
        return self.maxlink

    def getDieTime(self):
        return self.dieTime

    def __cmp__(self, other):
        return cmp(self.dieTime, other.dieTime)

    def __lt__(self, other):
        return self.dieTime < other.dieTime

    def __ge__(self, other):
        return self.dieTime >= other.dieTime

class Vertex:
    def __init__(self, w, v):
        self.weight = w
        self.vert = v

    def getV(self):
        return self.vert

    def getWeight(self):
        return self.weight

    def __cmp__(self, other):
        return cmp(self.weight, other.weight)

    def __lt__(self, other):
        return self.weight < other.weight

    def __ge__(self, other):
        return self.weight >= other.weight

def main():
    if len(sys.argv) != 6:
        print("Usage: python RoutingPerformance.py <network> <routing> <topology> <workload> <rate>\n")
        return

    if sys.argv[1] != 'CIRCUIT' and sys.argv[1] != 'PACKET':
        print("NETWORK_SCHEME must be CIRCUIT or PACKET")
        return

    if sys.argv[2] != 'SDP' and sys.argv[2] != 'LLP' and sys.argv[2] != 'SHP':
        print("ROUTING_SCHEME must be SDP or LLP or SHP")
        return

    #print(sys.argv[3])

    topdict = dict()

    #1.0 read in topology
    with open(sys.argv[3], 'r') as topfile:
        all_lines = topfile.readlines()

        for line in all_lines:
            line = line.strip()
            line = line.split(' ')

            src = line[0].strip()
            dst = line[1].strip()

            pro = int(line[2].strip())
            mal = int(line[3].strip())

            if src not in topdict:
                topdict[src] = {}

            if dst not in topdict:
                topdict[dst] = {}

            edge = Edge()
            edge.initLink(src, dst, pro, mal)

            topdict[src][dst] = edge
            topdict[dst][src] = edge

    #printTop(topdict)

    #2.0 read in workload and process
    processWorkload(topdict, sys.argv[2], sys.argv[4])

    return topdict

    # read in workload

def isAdjacent(topdict, i, j):
    if i in topdict and j in topdict[i]:
        return True
    return False

def update(scheme, dist, src, dst, topdict):
    if scheme == 'SDP':
        return dist + topdict[src][dst].getProbagation()

    elif scheme == 'LLP':
        rate = 1.0*topdict[src][dst].getRunTimeLink()/topdict[src][dst].getMaxLink()
        if dist < rate:
            return rate
        else:
            return dist

    else:
        return dist + 1

# v1: src node, v1: dst node
def dijkstra(topdict, v1, v2, scheme):

    st = dict((k, ' ') for k in topdict.keys())
    dist = dict((k, sys.maxsize) for k in topdict.keys())

    dist[v1] = 0

    pq = queue.PriorityQueue()

    vt = Vertex(dist[v1], v1)
    pq.put(vt)

    while not pq.empty():

        v = pq.get().getV()

        if dist[v] < sys.maxsize:
            for to in topdict[v].keys(): # edge from v

                value = update(scheme, dist[v], v, to, topdict)

                if scheme != 'LLP' and dist[to] == value:
                    randomNum = random.randint(0, 1)+1
                    if randomNum == 1:
                        st[to] = v
                elif dist[to] > value:
                    dist[to] = value 
                    vt1 = Vertex(dist[to], to)
                    pq.put(vt1)
                    st[to] = v

    stack = []
    temp = v2
    while temp != ' ':
        stack.append(temp)
        temp = st[temp]

    # print the route path
    stack.reverse()
    #print(stack)

    return stack

def processLink(a, b, topdict):
    if topdict[a][b].getRunTimeLink() < topdict[a][b].getMaxLink():
        return True
    else:
        return False

def inputLink(a, b, dietime):
    ed = Edge()
    ed.initDie(a, b, dietime)

    return ed

def processWorkload(topdict, scheme, loadfile):

    time = 0

    blocked = 0
    passed = 0
    total = 0
    totalHoc = 0
    totalprob = 0

    #1.0 read in workload and process
    with open(loadfile, 'r') as workload:
        all_lines = workload.readlines()

        dq = queue.PriorityQueue()

        for line in all_lines:
            line = line.strip()
            line = line.split(' ')

            v1 = line[1].strip()
            v2 = line[2].strip()

            start = float(line[0].strip())
            duration = float(line[3].strip())

            route = dijkstra(topdict, v1, v2, scheme)

            dietime = start + duration
            time = start
            total = total + 1

            top = None

            while not dq.empty():
                top = dq.get()
                if top.getDieTime() > start:
                    break

                a = top.getVert1()
                b = top.getVert2()

                #print(a)
                #print(b)

                topdict[a][b].decreaseRunTimeLink()

            if top is not None and top.getDieTime() > start:
                dq.put(top)

            subprob = 0
            i = 0
            #print(route)

            while route[i] != v2:
                if processLink(route[i], route[i+1], topdict):
                    subprob = subprob + topdict[route[i]][route[i+1]].getProbagation()
                else:
                    totalHoc = totalHoc - i
                    blocked = blocked + 1
                    break

                i = i + 1

            if route[i] == v2:
                i = 0
                while route[i] != v2:
                    topdict[route[i]][route[i+1]].increaseRunTimeLink()
                    dq.put(inputLink(route[i], route[i+1], dietime))
                    totalHoc = totalHoc + 1
                    i = i+1

                passed = passed + 1
                totalprob = totalprob + subprob

        rate = int(sys.argv[5])
        #print(rate)

        print("total number of virtual circuit requests: %d" % total)
        print("total number of packets: %d" % total)
        print("number of successfully routed packets: %d" % passed)
        print("percentage of routed packets: %f" % (passed/total*100.0))
        print("number of blocked packets: %d" % blocked)
        print("percentage of blocked packets: %f" % (blocked/total*100.0))
        print("average number of hops per circuit: %f" % (totalHoc*1.0/passed))
        print("average cumulative propagation delay per circuit: %f" % (totalprob*1.0/passed))
                

if __name__ == "__main__":
    topG = main()
