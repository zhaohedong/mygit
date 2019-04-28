#include <vector>
#include <string>
#include <iostream>
#include <iterator>
#include <stdlib.h>
#include <stdio.h>
#include <unordered_map>
using namespace std; 
struct TreeNode {
	int val;
	TreeNode *left;
	TreeNode *right;
	TreeNode(int x) : val(x), left(NULL), right(NULL) {}
};


class Solution {
public:
    static string tree2str(TreeNode* t) {
#if 0        
        if (t == nullptr) return "";
        string s = std::to_string(t->val);
        if(t->left)
        {
            s += "(";
            s += tree2str(t->left);
            s += ")";
        }
        
        if(t->right) 
        {
            if (t->left == NULL) s += "()";
            s += "(";
            s += tree2str(t->right);
            s += ")";
        }
        return s;
#elseif 0
        if (t == nullptr) return "";
        //string s;
        queue<TreeNode> q;
        q.push(*t);
        while(q.size() != 0)
        {
            int n = q.size();
            for(int i = 0; i < n; i++)
            {
                TreeNode tmp = q.front();
                q.pop();
                s += std::to_string(tmp.val);
                if(tmp.left != nullptr) q.push(*tmp.left);
                if(tmp.right != nullptr) q.push(*tmp.right);
            }
        }  
        return s;
#else
            string res;
            preTraverse(t,res);
            return res;
        }
        static void preTraverse(TreeNode* pt, string& s)
        {
            if (pt == nullptr) return ;
            s += to_string(pt -> val);
            if(pt->left || pt->right) s += "(", preTraverse(pt -> left, s), s += ")";
            if(pt->right) s += "(", preTraverse(pt -> right, s), s += ")";
        }
#endif
};

//int main(int argc,char *argv[])
int main()
{
 TreeNode* input = new TreeNode(1);
 TreeNode* input1 = new TreeNode(2);
 TreeNode* input2 = new TreeNode(3);
 TreeNode* input3 = new TreeNode(4);
 //TreeNode* input4 = new TreeNode(1);
 input->left = input1;
 input->right = input2; 
 input1->left = input3;
 //input3->left = input4;
  
 string ans = Solution::tree2str(input);
 int a = 0;
 int b = 0;
 if(1) a = 2, b = 3;
 cout <<  a << endl;
 cout <<  b << endl;
 cout << ans << endl;

 return 0;
}
