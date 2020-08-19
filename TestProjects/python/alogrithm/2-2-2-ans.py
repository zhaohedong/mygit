#!/usr/bin/python

listA = [2,3,1,5,6,4]
def sel_sort(listA):
    for i in range(0,len(listA)-1):
        key_min = i 
        for j in range(i+1,len(listA)):
            if listA[j] < listA[key_min]:
                key_min = j
        temp = listA[i]
        listA[i] = listA[key_min]
        listA[key_min] = temp
sel_sort(listA)
print listA
