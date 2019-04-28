#include <vector>
#include <string>
#include <iostream>
#include <iterator>
#include <stdlib.h>
#include<stdio.h>
#include <map>
using namespace std; 
struct stu
{
    char c;
    int i;
};
 int firstUniqChar(string s) {
        vector<stu> imap;
        bool bFind = false;
        for(char c:s)
        {
            bool bFind = false;
            for (auto it = imap.begin() ; it != imap.end(); ++it)
            {
                if(it->c == c)
                {
                    bFind = true;
                    it->i += 1;
                }
            }

            if (!bFind)
            {
                stu stu1 = {c,1};
                imap.push_back(stu1);
            }
            bFind = false;
        }
        
        char ans = 0;
        for (auto it = imap.begin() ; it != imap.end(); ++it)
        {
            if(it->i == 1)
            {
                ans = it->c;
            }
        }
        
        int ans_index = 0;
        for(int index = 0; index < s.length(); index++)
        {
            if(s[index] == ans) ans_index = index;
        }
        return ans_index;
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
 firstUniqChar("leetcode");
 return 0;
}
