#ifndef COPYLINKLIST_H
#define COPYLINKLIST_H

#include <utility>
#include <map>
#include <unordered_map>

using std::map;
using std::pair;
using std::unordered_map;

class Node {
public:
	int val;
	Node* next;
	Node* random;

	Node(int _val) {
		val = _val;
		next = NULL;
		random = NULL;
	}
};

Node* copyRandomList(Node* head) {
	if (!head) {
		return nullptr;
	}

	//copy val
	Node* pos = head;
	map<Node*, Node*> list;
	while (pos) {
		list.insert(make_pair(pos, new Node(pos->val)));
		pos = pos->next;
	}

	//copy random
	pos = head;
	while (pos) {
		auto iterPre = list.find(pos);
		auto iterNex = list.find(pos->next);
		if (iterNex != list.end())
			iterPre->second->next = iterNex->second;
		auto iterRan = list.find(pos->random);
		if (iterRan != list.end())
			iterPre->second->random = iterRan->second;
		pos = pos->next;
	}

	return list.find(head)->second;
}

#endif
