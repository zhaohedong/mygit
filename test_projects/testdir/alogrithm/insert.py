#!/usr/bin/python

listA = [5,2,4,6,1,3]

def insert_sort(listA):
    for i in range(1,len(listA)):
        j = i - 1
        key = listA[i]
        while j>= 0 and listA[j] > key:
            listA[j+1] = listA[j]
            j = j -1
        listA[j+1] = key

insert_sort(listA)
print listA
