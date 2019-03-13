#include <vector>
#include <string>
#include <iostream>
using namespace std; 
class Solution {
public:
    static vector<string> findWords(vector<string>& words) {
        string l1 = "qwertyuiop";
        string l2 = "asdfghjklDA";
        string l3 = "zxcvbnm";
        char result = 0x00;
        vector<string> rlt;
        
        for (int i = 0; i < words.size(); i++)
        {
	    result = 0x00;
            string temp = words[i];
            for (int j = 0; j < temp.length(); j++)    
            {   if(temp.at(j) < 'a')
                {
                    char c = temp.at(j)+32;
                    temp = temp.erase(j,1);
                    temp = temp.insert(j,1,c);
                }
                if(l1.find(temp.at(j)) != -1) {
                    result = result | 0x01;
		    if (result != 0x01)
		    {
		    	continue;
		    }
                }
                if(l2.find(temp.at(j)) != -1) {
                    result = result | 0x02;
		    if (result != 0x02)
		    {
		    	continue;
		    }
                }
                if(l3.find(temp.at(j)) != -1) {
                    result = result | 0x04;
		    if (result != 0x04)
		    {
		    	continue;
		    }
                }
            }
            if (result == 0x01 || result == 0x02 || result == 0x04){
                rlt.push_back(words[i]);
            }
        }
        return rlt;
    }
};

int main()
{

 vector<string> input; 
 input.push_back("Hello");
 input.push_back("Alaska");
 input.push_back("Dad");
 input.push_back("Peace");
 vector<string> output = Solution::findWords(input);
 cout << input.at(0) << endl;
 cout << output.size() << endl;
 cout << output.at(0) << endl;
 return 0;
}
