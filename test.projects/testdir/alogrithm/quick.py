#!/usr/bin/python

A = [2,3,5,7,1,4,8,6]

def quick_sort(A,p,r):
    if p < r:
        q = quick_partion(A,p,r)
        quick_sort(A,p,q-1)
        quick_sort(A,q+1,r)

def quick_partion(A,p,r):
    x = A[r]
    i = p - 1
    tmp = 0
    for j in range(p,r):
        if A[j] <= x:
            i += 1
            tmp = A[i]
            A[i] = A[j]
            A[j] = tmp
        j += 1
    tmp = A[i+1]
    A[i+1] = A[r] 
    A[r] = tmp 
    print A
    return i+1

quick_sort(A,0,len(A)-1)
print A


