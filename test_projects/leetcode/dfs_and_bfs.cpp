/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode(int x) : val(x), left(NULL), right(NULL) {}
 * };
 */
#include <vector>
using namespace std;
class Solution {
public:
    //vector<stu1> tmp;
#ifdef DFS
    int average(TreeNode* root, int level, vector<double>& sum, vector<int>& counter)
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
    vector<double> averageOfLevels(TreeNode* root) {
        vector<double> res;
        vector<int> counter;
        vector<double> nullrlt(1,0.0);
        if (root == NULL) 
            return nullrlt;
        vector<double> rlt;
        average(root, 0, res, counter);
        for(int i = 0; i < res.size(); i++){
            rlt.push_back(res[i]/counter[i]);
        }
        return rlt;
    }
#else //BFS
     vector<double> averageOfLevels(TreeNode* root) {
        vector<double> rlt;
        queue<TreeNode> q;
        q.push(*root);
        while(q.size() != 0)
        {
            int n = q.size();
            double val = 0.0;
            for(int i = 0; i < n; i++)
            {
                TreeNode node = q.front();
                q.pop();
                val += node.val;
                if (node.left != NULL) q.push(*node.left);
                if (node.right != NULL) q.push(*node.right);
            }
            rlt.push_back(val/n);
        }
        return rlt;
    }
#endif
};
