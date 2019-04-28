#!/usr/bin/python

#graph table
graph = {}
graph["S"] = {}
graph["S"]["A"] = 5 
graph["S"]["D"] = 2

graph["A"] = {}
graph["A"]["B"] = 4
graph["A"]["C"] = 2

graph["D"] = {}
graph["D"]["A"] = 8
graph["D"]["C"] = 7

graph["B"] = {}
graph["B"]["E"] = 3
graph["B"]["C"] = 6

graph["C"] = {}
graph["C"]["E"] = 1

graph["E"] = {}

#cost table
infinity = float("inf")

cost = {}
cost["A"] = 5 
cost["D"] = 2 
cost["C"] = infinity
cost["B"] = infinity
cost["E"] = infinity

#parents table
parent = {}
parent["A"] = "S"
parent["D"] = "S"
parent["C"] = {}
parent["B"] = {}
parent["E"] = {}

#processed
processed = [] 

def lowest_cost(cost):
    lowest = infinity
    n = None
    for i in cost:
        if cost[i] < lowest and i not in processed:
            lowest = cost[i]
            n = i
    print n 
    return n

node = lowest_cost(cost)
while node != None:
    neighbors = graph[node]
    for key in neighbors.keys():
        if neighbors[key] + cost[node] < cost[key]:
            cost[key] = neighbors[key] + cost[node]
            parent[key] = node
    processed.append(node)
    node = lowest_cost(cost)
print cost
print parent 




