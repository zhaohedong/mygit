#!/usr/bin/python

listA = [2,3,1,5,6,4]
def sel_sort(listA):
    for j in range(1,len(listA)):
        i = j -1
        key = listA[i]
        index = 0
        xchg = False
        while i < len(listA): 
            if key > listA[i]:
                key = listA[i]
                index = i
                print "i=%s listA=%s key=%s" %(i, listA[i], key)
                xchg = True
            i += 1
        if xchg == True:
            xchg = False
            listA[index] = listA[j-1]
            listA[j-1] = key
        print listA
sel_sort(listA)
print listA
