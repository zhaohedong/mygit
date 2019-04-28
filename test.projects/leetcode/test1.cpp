#include <vector>
#include <string>
#include <iostream>
using namespace std; 

class Solution {
public:
    static string reverseWords(string s) {
    	string strrlt = "";
        for (int i = 0; i < s.length(); i++){
            string tmpstr = "";

            if (s.at(i) == ' '){
                strrlt += ' ';
                continue;
            }

           while(i < s.length() && s.at(i) != ' ' ){
                tmpstr += s.at(i);
                i++;
           }
           int j = tmpstr.length() - 1;
            if (j >= 0){
                while(j >= 0) {
                    strrlt += tmpstr.at(j--);
                }
                if (i < s.length()){
                    strrlt += ' ';
                }
            }
        }
        return strrlt;
    }
};

int main()
{

 string input = "I love y";
 string rlt = Solution::reverseWords(input);
 cout << rlt << endl;
 return 0;
}
