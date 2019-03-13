#include <vector>
#include <string>
#include <iostream>
#include <iterator>
#include <stdlib.h>
#include <stdio.h>
#include <unordered_map>
using namespace std; 

char findTheDifference(string s, string t) {
        unordered_map<char, int> imap;
        for(int i = 0; i < s.length(); i++)
        {
            if (imap.find(s[i]) != imap.end())
            {
                imap[s[i]] += 1;
            }
            else
            {
                imap.insert(make_pair(s[i],1));
            }
        }
        
        unordered_map<char, int> jmap;
        for(int i = 0; i < t.length(); i++)
        {
            if (jmap.find(t[i]) != jmap.end())
            {
                jmap[t[i]] += 1;
            }
            else
            {
                jmap.insert(make_pair(t[i],1));

            }
        }
        
        unordered_map<char, int>::iterator iter;
        for (iter = jmap.begin(); iter != jmap.end(); ++iter)
        {
            	std::unordered_map<char,int>::const_iterator got = imap.find(iter->first);
            	if (got->second != iter->second || got == imap.end()) 
	    	{
		    return iter->first;
		}
        }
       // return t[t.length() - 1];
        return 'A';
}

//int main(int argc,char *argv[])
int main()
{
 char rlt = findTheDifference("abcd","abcde");
 cout << rlt << endl;
 return 0;
}
