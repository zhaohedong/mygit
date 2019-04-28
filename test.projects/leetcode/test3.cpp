#include <vector>
#include <string>
#include <iostream>
using namespace std; 


struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode(int x) : val(x), left(NULL), right(NULL) {}
};
class Solution {
public:
    static TreeNode* trimBST(TreeNode* root, int L, int R) {
        TreeNode* parent = root;
	if(parent)
        {
            TreeNode* lchild = parent->left;
            TreeNode* rchild = parent->right;   

            if(lchild)
            {
                if(lchild->val > R)
                {
                    parent->left = lchild->left;
                }
                else if(lchild->val < L)
                {
                    parent->left = lchild->right;
                }

                if (lchild)
                {
                    trimBST(parent->left,L,R);
                }
            }
            
            if(rchild)
            {
                if(rchild->val > R)
                {
                    parent->right = rchild->left;
                }
                else if(rchild->val < L)
                {
                    parent->right = rchild->right;
                }

                if (rchild)
                {
                    trimBST(parent->right,L,R);
                }
            }
        }
	if(parent){
 		if (parent->val > R)
		{
		   parent = parent->left;
		}
		else if (parent->val < L)
		{
		   parent = parent->right; 
		}
	}
        return parent;
  }
  static int show(TreeNode* root)
  {
  	if (root)
	{
		cout << root->val << endl;
		show(root->left);
		show(root->right);
	}
	return 0;
  }

};

int main()
{

 TreeNode* input = new TreeNode(3);
 TreeNode* input1 = new TreeNode(0);
 TreeNode* input2 = new TreeNode(4);
 TreeNode* input3 = new TreeNode(2);
 TreeNode* input4 = new TreeNode(1);
 input->left = input1;
 input->left = input1;
 input->right = input2; 
 input1->right = input3;
 input3->left = input4;
  
 TreeNode* rlt = Solution::trimBST(input,1,2);
 Solution::show(rlt);
 return 0;
}
