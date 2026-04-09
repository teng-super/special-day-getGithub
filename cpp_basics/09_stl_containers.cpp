/**
 * 09_stl_containers.cpp
 * C++ 入门：STL 常用容器
 * 演示 vector、list、deque、map、unordered_map、set、stack、queue
 */

#include <iostream>
#include <vector>
#include <list>
#include <deque>
#include <map>
#include <unordered_map>
#include <set>
#include <stack>
#include <queue>
#include <algorithm>
using namespace std;

int main() {
    // ---------- vector：动态数组 ----------
    cout << "=== vector ===" << endl;
    vector<int> vec = {3, 1, 4, 1, 5, 9, 2, 6};
    vec.push_back(7);
    cout << "大小: " << vec.size() << "，容量: " << vec.capacity() << endl;
    sort(vec.begin(), vec.end());
    for (int v : vec) cout << v << " ";
    cout << endl;
    vec.erase(vec.begin());  // 删除首元素
    cout << "删除首元素后: ";
    for (int v : vec) cout << v << " ";
    cout << endl;

    // ---------- list：双向链表 ----------
    cout << "\n=== list ===" << endl;
    list<int> lst = {10, 20, 30, 40, 50};
    lst.push_front(5);
    lst.push_back(60);
    lst.remove(30);   // 删除值为 30 的所有元素
    for (int v : lst) cout << v << " ";
    cout << endl;

    // ---------- deque：双端队列 ----------
    cout << "\n=== deque ===" << endl;
    deque<int> dq = {2, 3, 4};
    dq.push_front(1);
    dq.push_back(5);
    for (int v : dq) cout << v << " ";
    cout << endl;

    // ---------- map：有序键值对 ----------
    cout << "\n=== map ===" << endl;
    map<string, int> scores;
    scores["Alice"] = 95;
    scores["Bob"]   = 82;
    scores["Carol"] = 90;
    scores.insert({"Dave", 88});
    for (const auto& [name, score] : scores) {  // C++17 结构化绑定
        cout << name << ": " << score << endl;
    }
    cout << "Alice 的分数: " << scores["Alice"] << endl;
    scores.erase("Bob");
    cout << "删除 Bob 后大小: " << scores.size() << endl;

    // ---------- unordered_map：哈希表（查找 O(1)） ----------
    cout << "\n=== unordered_map ===" << endl;
    unordered_map<string, int> umap;
    umap["apple"]  = 3;
    umap["banana"] = 5;
    umap["cherry"] = 1;
    for (const auto& [key, val] : umap) {
        cout << key << " -> " << val << endl;
    }

    // ---------- set：有序不重复集合 ----------
    cout << "\n=== set ===" << endl;
    set<int> s = {5, 3, 8, 1, 9, 3, 5};  // 重复元素自动去重
    for (int v : s) cout << v << " ";
    cout << endl;
    cout << "包含 3? " << boolalpha << (s.count(3) > 0) << endl;

    // ---------- stack：LIFO ----------
    cout << "\n=== stack ===" << endl;
    stack<int> stk;
    stk.push(10); stk.push(20); stk.push(30);
    while (!stk.empty()) {
        cout << stk.top() << " ";
        stk.pop();
    }
    cout << endl;

    // ---------- queue：FIFO ----------
    cout << "\n=== queue ===" << endl;
    queue<string> q;
    q.push("任务1"); q.push("任务2"); q.push("任务3");
    while (!q.empty()) {
        cout << q.front() << " ";
        q.pop();
    }
    cout << endl;

    // ---------- priority_queue：优先队列（默认最大堆） ----------
    cout << "\n=== priority_queue ===" << endl;
    priority_queue<int> pq;
    pq.push(5); pq.push(1); pq.push(8); pq.push(3);
    while (!pq.empty()) {
        cout << pq.top() << " ";
        pq.pop();
    }
    cout << endl;

    return 0;
}
