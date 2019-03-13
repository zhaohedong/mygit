#include <vector>
#include <string>
#include <iostream>
#include <iterator>
#include <stdlib.h>
#include <stdio.h>
#include <unordered_map>
using namespace std; 

void moveZeroes(vector<int>& nums) {
        int count = 0;
        for (std::vector<int>::iterator it = nums.begin(); it != nums.end(); it++)
        {
            if(*it == 0)
            {
                nums.erase(it);
                count++;
            }
        }
        while(count--)
        {
            nums.push_back(0);
        }
}

//int main(int argc,char *argv[])
int main()
{
 vector<int> input(1,0);
 moveZeroes(input);
 //cout << rlt << endl;
 return 0;
}
