#pragma once
#include <cstdio>
#include <cstring>
#include <iostream>
#include <string>
#include <stack>
#include <list>
using namespace std;

//字符串反转输出
void Reserve() {
    char in[1024];
    stack<char> outStack;
    cin >> in;

    for (int i = 0; i < strlen(in); i++) {
        outStack.push(in[i]);
    }
    while (outStack.empty()) {
        cout << outStack.top();
        outStack.pop();
    }
}

struct Node {
    int x;
    int y;
    Node* pre;

    Node(int a, int b, Node* c = nullptr) : x(a), y(b), pre(c) {}

    //bool operator==(Node& n) {
    //    if (this->x == n.x && this->y == n.y) {
    //        return true;
    //    }
    //    else {
    //        return false;
    //    }
    //}
    //
    //Node& operator+=(Node& n) {
    //    this->x += n.x;
    //    this->y += n.y;
    //    return *this;
    //}
    //
    //Node operator+(Node& n) {
    //    Node temp(0, 0);
    //    temp.x += n.x;
    //    temp.y += n.y;
    //    return temp;
    //}

};

//判断点是否越界
bool inArea(Node& n, int N, int M) {//判断是否越界
    return n.x >= 0 && n.x < N && n.y >= 0 && n.y < M;
}

//广度遍历
void BFS() {
    //输入
    int N, M;
    cin >> N >> M;
    int tu[10][10];//读取输入数组
    int isPass[10][10];//记录是否已访问，0未访问，1已访问
    memset(tu, -1, 100 * sizeof(int));
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            cin >> tu[i][j];
            isPass[i][j] = 0;
        }
    }

    Node direction[4] = { Node(1,0), Node(0,1), Node(-1,0), Node(0,-1) };//四个方向，下右上左
    list<Node*> queue;//访问队列
    Node* cur = new Node(0, 0);
    Node* end = new Node(N - 1, M - 1);
    queue.push_back(cur);
    while (queue.size()) {
        cur = queue.front();
        isPass[cur->x][cur->y] = 1;//已访问
        queue.pop_front();

        if (cur->x == end->x && cur->y == end->y) {//是否到达终点
            break;
        }

        for (int i = 0; i < 4; i++) {
            Node* temp = new Node(cur->x + direction[i].x, cur->y + direction[i].y);
            temp->pre = cur;//记录前一节点
            if (inArea(*temp, N, M)//是否越界
                && 1 != tu[temp->x][temp->y]//是否是墙
                && 0 == isPass[temp->x][temp->y])//是否已访问
            {
                queue.push_back(temp);

            }
            else {
                delete temp;
                temp = nullptr;
            }
        }//for
    }
    delete end;
    end = nullptr;

    //输出
    std::stack<Node*> printStack;
    Node* last = cur;
    while (last) {
        printStack.push(last);
        last = last->pre;
    }
    while (printStack.size()) {
        Node* p = printStack.top();
        cout << "(" << p->x << p->y << ")" << endl;
        printStack.pop();
    }

    //析构
    while (cur) {
        Node* t = cur->pre;
        delete cur;
        cur = t;
    }
}

class findNode {
public:
    findNode(Node& i) : it(i.x, i.y) {}
    bool operator()(Node& i) {
        return it.x == i.x && it.y == i.y;
    }
private:
    Node it;
};

struct ListNode {
    int val;
    ListNode* next;
    ListNode(int x) : val(x), next(nullptr) {}
};

//反转链表
class ReverseList {
public:
    ListNode* reverseList(ListNode* head) {
        if (!head || !head->next) {
            return head;
        }

        ListNode* pre = nullptr;
        ListNode* mid = head;
        ListNode* end = head->next;
        while (end) {
            mid->next = pre;
            pre = mid;
            mid = end;
            end = mid->next;
        }
        mid->next = pre;
        return mid;
    }
};