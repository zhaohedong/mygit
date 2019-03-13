#!/usr/bin/python
import pprint
import time
from time import clock
from timeit import Timer
import math

start = clock()
cell = [[0,0,0,0,0,0,0,0],[0,0,0,0,0,0,0,0],[0,0,0,0,0,0,0,0],[0,0,0,0,0,0,0,0]]

#goods = {'iphone':'2000','sounder':'3000','computer':'2000','guiter':'1500'}
price = [1500,3000,2000,1000]
weight = [1,4,3,0.5]

def func():
    for i in range (len(weight)):
        for j in range(len(cell[0])):
            if (j+1)/2.0 < weight[i]:
                if i != 0:
                    cell[i][j] = cell[i-1][j]
                else:
                    cell[i][j] = 0
            elif (j+1)/2.0 == weight[i]:
                cell[i][j] = max(price[i],cell[i-1][j])
            else:
                cell[i][j] = max(price[i]+cell[i-1][int(((j+1)/2.0-weight[i]-0.5)*2)],cell[i-1][j])
func()
print cell
#finish = clock()


#if __name__=='__main__':
#    t1=Timer("func()","from __main__ import func")
#    print t1.timeit(10000)
#    print t1.repeat(3,10000)
#

#print (finish-start)/10000000000

