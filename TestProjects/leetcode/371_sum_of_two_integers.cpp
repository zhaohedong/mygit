#include <vector>
#include <string>
#include <iostream>
#include <iterator>
#include <stdlib.h>
#include<stdio.h>
using namespace std; 

int getSum(int a, int b) 
{
   int sum = a;
   while (b != 0)
   {
       sum = a ^ b;
       b = (a & b) << 1;
       a = sum;
   }
   return sum;
}

//int main(int argc,char *argv[])
int main()
{
 int a = 0b0011;
 int b = 0b1111;
// string s1(argv[1]);
// string s2(argv[2]);
// sscanf(argv[1],"%d",&a);
// sscanf(argv[2],"%d",&b);
// a = stoi(s1);
// b = stoi(s2);
 int rlt = getSum(a, b);
 cout << rlt << endl;
 return 0;
}
