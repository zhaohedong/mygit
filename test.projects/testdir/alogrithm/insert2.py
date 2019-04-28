#!/usr/bin/python

listA = [2,3,5,7,1,4,8,6]

def insert_sort(listA):
    for i in range(1, len(listA)):
        j = i - 1
        index = 0
        key = listA[i]
        for k in range(j,-1,-1):
            if listA[k] < key:
               index = k+1
               break
            else:
               listA[k+1] = listA[k]
        listA[index] = key        
        print "index = %s key = %s" %(index,key)
        print listA   
print listA
insert_sort(listA)
