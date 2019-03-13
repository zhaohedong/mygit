#!/usr/bin/python

listA = [31, 41, 59, 26, 41, 58]

def insert_sort(listA):
    for i in range(1,len(listA)):
        key = listA[i]
        j = i - 1
        while listA[j] < key and j >= 0:
            listA[j+1] = listA[j]
            j -= 1
        listA[j+1] = key

insert_sort(listA)
print listA




