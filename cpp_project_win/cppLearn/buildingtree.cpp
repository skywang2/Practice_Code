#include <vector>
using namespace std;

struct TreeNode {
	int val;
	TreeNode *left;
	TreeNode *right;
	TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
	
};

TreeNode* buildTree(vector<int>& inorder, vector<int>& postorder) {
	return fun(inorder, postorder, 0, inorder.size() - 1);
}

TreeNode* fun(vector<int>& inorder,
	vector<int>& postorder,
	int in_begin,
	int in_end) {
	if (in_begin > in_end)
		return NULL;
	TreeNode* root = (TreeNode*)malloc(sizeof(TreeNode));
	root->val = postorder.back();
	postorder.pop_back();
	root->right = fun(inorder, postorder, inorder.find(root->val) + 1, in_end);//假设查询序号函数是find()
	root->left = fun(inorder, postorder, in_begin, inorder.find(root->val) - 1);
}

