#ifndef TEMPLATE_TEST_H
#define TEMPLATE_TEST_H

template<typename valType>
class BTnode
{
public:

private:
	valType _val;
	int _cnt;
	BTnode* _lchild;
	BTnode* _rchild;
};

template<typename elemType>
class BinaryTree
{
public:
	BinaryTree();
	BinaryTree(const BinaryTree&);
	~BinaryTree();
	BinaryTree& operator=(const BinaryTree&);

	bool empty(){ return _root == nullptr; }
	void clear();
private:
	BTnode<elemType>* _root;
	//void copy(BTnode<elemType>* tar, BTnode<elemType>* src);
};

template<typename elemType>
BinaryTree<elemType>::BinaryTree() : _root(nullptr)
{ }

template<typename elemType>
BinaryTree<elemType>::BinaryTree(const BinaryTree&)
{ }

template<typename elemType>
BinaryTree<elemType>::~BinaryTree()
{ }

template<typename elemType>
BinaryTree<elemType>& BinaryTree<elemType>::operator=(const BinaryTree& rhs)
{ 
	return *this;
}

#endif
