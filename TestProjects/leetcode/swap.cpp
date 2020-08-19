/******************************************************************* 
 * Copyright (C) Jerry Jiang 
 *                
 * File Name   : swap.cpp 
 * Author      : Jerry Jiang 
 * Create Time : 2012-3-24 4:19:31 
 * Mail        : jbiaojerry@gmail.com 
 * Blog        : http://blog.csdn.net/jerryjbiao  
 *                
 * Description : 简单的程序诠释C++ STL算法系列之十五                   
 *               成员函数swap实现容器的内存释放    
 *                
 ******************************************************************/  
  
#include <iostream>  
#include <algorithm>  
#include <vector>  
#include <iterator>  
  
using namespace std;  
  
int main ()   
{  
    int x = 10;  
    vector<int> myvector(10000, x);    
  
    //这里打印仅仅是元素的个数不是内存大小  
    cout << "myvector size:"  
         << myvector.size()  
         << endl;  
 
    vector<int>::iterator it = myvector.begin();
    while(it != myvector.end()) myvector.erase(it);
    cout << "after erase :"  
         << myvector.size()  
         << endl;  
  
    //swap交换函数释放内存：vector<T>().swap(X);  
    //T:int ; myvertor代表X  
    vector<int>().swap(myvector);  
  
    //两个输出仅用来表示swap前后的变化  
    cout << "after swap :"  
         << myvector.size()  
         << endl;  
  
    return 0;  
}  
