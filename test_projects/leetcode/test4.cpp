#include <vector>
#include <string>
#include <iostream>
using namespace std; 
vector<double> sum;
vector<int> counter;
 


struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode(int x) : val(x), left(NULL), right(NULL) {}
};
class Solution {
public:
    public:
    //map<int,int> tmp；
        static int average(TreeNode* root, int level, vector<double>& sum, vector<int>& counter)
    {
        if (root == NULL) return 0;
        if(level < sum.size())
        {
            sum[level] += root->val;
            counter[level] += 1;
        }
        else
        {
            sum.push_back(root->val);
            counter.push_back(1);
        }
        average(root->left, level+1, sum, counter);
        average(root->right, level+1, sum, counter);
        return 0;
    }
    static vector<double> averageOfLevels(TreeNode* root) {
       vector<double> nullrlt(1,0.0);
        if (root == NULL) 
            return nullrlt;
        vector<double> rlt;
        average(root, 0, sum, counter);
        for(int i = 0; i < sum.size(); i++){
            rlt.push_back(sum[i]/counter[i]);
        }
	show(rlt);
        return rlt;
    }
  static int show(vector<double> rlt)
  {
       for(int i = 0; i < rlt.size(); i++){
            cout<<rlt[i]<<endl;
        }
        return 0;
  }

};

int main()
{

 TreeNode* input = new TreeNode(3);
 TreeNode* input1 = new TreeNode(9);
 TreeNode* input2 = new TreeNode(20);
 TreeNode* input3 = new TreeNode(15);
 TreeNode* input4 = new TreeNode(7);
 input->left = input1;
 input->right = input2; 
 input2->right = input4;
 input2->left = input3;
  
 Solution::averageOfLevels(input);
 return 0;
}
