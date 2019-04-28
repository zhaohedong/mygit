#include <vector>
#include <string>
#include <iostream>
#include <iterator>
#include <stdlib.h>
#include <stdio.h>
#include <unordered_map>
using namespace std; 

int maxCount(int m, int n, vector<vector<int>>& ops) {
	if (ops.size() == 0) return m*n;
	if (ops.size() == 1) return m*n;
        vector<vector<int>> M(m, vector<int>(n, 0));
        for( int i = 0; i < ops.size(); i++ )
        {   
            for(int j = 0; j < ops[i][0]; j++)
            {
                for(int k = 0; k < ops[i][1]; k++)
                {
                    M[j][k] += 1;
                }           
            }
        }
        
        int max = 0;
        int count = 0;
        for( int i = 0; i < m; i++ )
        {   
            for(int j = 0; j < n; j++)
            {
                if (max < M[i][j]) 
                {
                    max = M[i][j];
                    count = 1;
                }
                else if (max == M[i][j]) 
                {
                    count += 1;
                }
                else
                {
                    
                }  
            }
        }
	return count;
}

//int main(int argc,char *argv[])
int main()
{
 //vector<vector<int>> M(2, vector<int>(2, 0));
 vector<vector<int>> M(1, vector<int>(2,19999));
 /*M[0][0] = 2;
 M[0][1] = 2;
 M[1][0] = 3;
 M[1][1] = 3;*/
 int rlt = maxCount(39999,39999,M);
 cout << rlt << endl;
 return 0;
}
