#include<iostream>
#include<vector>
#include<malloc.h>
#include<queue>
#include<math.h>
using namespace std;

struct TreeNode {
	int val;
	TreeNode *left;
	TreeNode *right;
	TreeNode(int x):val(x), left(NULL), right(NULL){}
};

class Solution {
public:
	TreeNode* createTree() {
		TreeNode *p;
		int data;
		cin >> data;
		if (data == 0) {
			p = NULL;
		}
		else {
			p = (TreeNode *)malloc(sizeof(TreeNode));
			if (p) {
				p->val = data;
				p->left = createTree();
				p->right = createTree();
			}
		}
		return p;
	}

	void preorder(TreeNode* root, vector<int> &path) {
		if (root) {
			path.push_back(root->val);
			preorder(root->left, path);
			preorder(root->right, path);
		}
	}

	vector<int> preorderTraversal(TreeNode* root) {
		vector<int> path;
		preorder(root, path);
		return path;
	}

	void inorder(TreeNode* root, vector<int> &path) {
		if (root) {
			inorder(root->left, path);
			path.push_back(root->val);
			inorder(root->right, path);
		}
	}

	vector<int> inorderTraversal(TreeNode* root) {
		vector<int> path;
		inorder(root, path);
		return path;
	}


	void postorder(TreeNode* root, vector<int> &path) {
		if (root) {
			postorder(root->left, path);
			postorder(root->right, path);
			path.push_back(root->val);
		}
	}

	vector<int> postorderTraversal(TreeNode* root) {
		vector<int> path;
		postorder(root, path);
		return path;
	}

	vector<vector<int>> levelOrder(TreeNode* root) {
		TreeNode* p = NULL;
		vector<vector<int>> set; 
		if (!root)return set;

		queue<TreeNode *> q;
		q.push(root);
		while (!q.empty()) {
			vector<int> subset;
			int size = q.size();	//用size限制当前层的循环次数
			for (int i = 0; i < size; i++) {
				p = q.front();q.pop();	
				subset.push_back(p->val);	
				if (p->left)q.push(p->left);	
				if (p->right)q.push(p->right);	
			}

			set.push_back(subset);
		}

		return set;
	}
};