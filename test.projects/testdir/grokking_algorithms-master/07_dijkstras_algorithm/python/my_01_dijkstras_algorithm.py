#!/usr/bin/python

#graph table
graph = {}
graph["S"] = {}
graph["S"]["A"] = 6
graph["S"]["B"] = 2

graph["A"] = {}
graph["A"]["E"] = 1

graph["B"] = {}
graph["B"]["A"] = 3 
graph["B"]["E"] = 5

graph["E"] = {}

#cost table
infinity = float("inf")

cost = {}
cost["A"] = 6
cost["B"] = 2
cost["E"] = infinity

#parents table
parent = {}
parent["A"] = "S"
parent["B"] = "S"
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




