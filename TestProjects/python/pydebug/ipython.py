#/usr/bin/python
from __future__ import print_function
import crash_on_ipy

def sum_nums(n):
    s=0
    for i in range(n):
        s +=i 
        s = s/0
        print (s)
if __name__ == '__main__':
    sum_nums(5)


