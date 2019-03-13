#!/usr/bin/python

listA = [8,7,6,5,4,3,2,1]
arr = [0,0,0,0,0,0,0,0]

def merge_sort(array, l, r, tmp):
    if l + 1 < r:
        m = (l + r) / 2
        merge_sort(array, l, m, tmp)
        merge_sort(array, m, r, tmp)
        combine(array, l, m, r, tmp)

def combine(array, l, m, r, tmp):
    i = l
    k = 0
    j = m
    while(i < m and j < r):
        if(array[i] < array[j]):
            tmp[k] = array[i]
            i += 1
            k += 1
        else:
            tmp[k] = array[j]
            k += 1
            j += 1
    while i < m:
            tmp[k] = array[i]
            k += 1
            i += 1
    while j < r:
            tmp[k] = array[j]
            k += 1
            j += 1
    kk = 0
    ll = l
    while kk < k:
        array[ll] = tmp[kk]
        kk += 1
        ll += 1
    print array

print listA
merge_sort(listA, 0, 8, arr)
print listA

