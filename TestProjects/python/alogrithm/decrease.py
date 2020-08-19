#!/usr/bin/python
listA = [1,2,3,4,5,6,7,8]
listB = [0,0,0,0,0,0,0,0]

def merge(listA,l,r,listB):
    if l + 1 < r:
        m = (l+r)/2
        merge(listA,l,m,listB)
        merge(listA,m,r,listB)
        combine(listA,l,m,r,listB)

def combine(listA,l,m,r,listB):
    i = l
    j = m
    k = 0
    while i < m and j < r:
        if listA[i] > listA[j]:
            listB[k] = listA[i]
            i += 1
        else:
            listB[k] = listA[j]
            j += 1
        k += 1
    while i < m:
        listB[k] = listA[i]
        i += 1
        k += 1
    while j < r:
        listB[j] = listA[j]
        j += 1
        k += 1

    kk = 0
    ll = l
    while kk < k:
        listA[ll] = listB[kk]
        ll += 1
        kk += 1
    
merge(listA,0,8,listB)
print listB











