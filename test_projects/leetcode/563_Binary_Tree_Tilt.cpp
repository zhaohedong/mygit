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



static int tilt_sum = 0;
class Solution {
public:
    static int findTilt(TreeNode* root) {
        if(root == nullptr) 
        {
            return tilt_sum;
        }
        
        if(root->left != nullptr && root->right != nullptr)
        {
            findTilt(root->left);
            findTilt(root->right);
            tilt_sum += abs(root->left->val - root->right->val);
        }
        else if(root->left != nullptr && root->right == nullptr)
        {
            findTilt(root->left);
            tilt_sum += abs(root->left->val);
        }
        else if(root->left == nullptr && root->right != nullptr)
        {
            findTilt(root->right);
            tilt_sum += abs(root->right->val);
        }
        else
        {
            
        }
        return tilt_sum;
    }
};

//int main(int argc,char *argv[])
int main()
{
 TreeNode* input = new TreeNode(1);
 TreeNode* input1 = new TreeNode(2);
 //TreeNode* input2 = new TreeNode(3);
 //TreeNode* input3 = new TreeNode(4);
 //TreeNode* input4 = new TreeNode(1);
 input->left = input1;
// input->right = input2; 
// input1->left = input3;
 //input3->left = input4;
  
 int ans = Solution::findTilt(input);
 cout << ans << endl;

 return 0;
}
