/**
 * 【C++ 中级 - 第4课：STL 标准模板库】
 *
 * 知识点：
 * 1. 顺序容器：vector, list, deque, array
 * 2. 关联容器：map, multimap, set, multiset
 * 3. 无序关联容器（C++11）：unordered_map, unordered_set
 * 4. 容器适配器：stack, queue, priority_queue
 * 5. 迭代器（Iterator）：begin/end, rbegin/rend, cbegin/cend
 * 6. 算法（Algorithm）：sort, find, count, transform, accumulate 等
 * 7. 函数对象（Functor）和 lambda（C++11）
 *
 * 练习：
 *   - 单词频率统计（用 unordered_map）
 *   - 实现一个简单的任务队列
 */

#include <iostream>
#include <vector>
#include <list>
#include <deque>
#include <map>
#include <unordered_map>
#include <set>
#include <unordered_set>
#include <stack>
#include <queue>
#include <algorithm>
#include <numeric>      // accumulate
#include <functional>   // greater, less
#include <string>
#include <sstream>

using namespace std;

// ──────────────────────────────────────────────────────────────────
// 辅助：打印 map
template <typename K, typename V>
void print_map(const map<K, V>& m, const string& label = "") {
    if (!label.empty()) cout << label << ": ";
    cout << "{";
    bool first = true;
    for (const auto& [k, v] : m) {  // C++17 结构化绑定
        if (!first) cout << ", ";
        cout << k << ":" << v;
        first = false;
    }
    cout << "}" << endl;
}

int main() {
    // ─── 1. vector ────────────────────────────────────────────────
    cout << "=== vector ===" << endl;
    vector<int> v = {5, 2, 8, 1, 9, 3, 7, 4, 6};
    cout << "原始: ";
    for (int x : v) cout << x << " ";
    cout << endl;

    sort(v.begin(), v.end());           // 升序
    cout << "升序: ";
    for (int x : v) cout << x << " ";
    cout << endl;

    sort(v.begin(), v.end(), greater<int>());  // 降序
    cout << "降序: ";
    for (int x : v) cout << x << " ";
    cout << endl;

    // 算法示例
    cout << "总和: " << accumulate(v.begin(), v.end(), 0) << endl;
    cout << "最大: " << *max_element(v.begin(), v.end()) << endl;
    cout << "最小: " << *min_element(v.begin(), v.end()) << endl;

    // find
    auto it = find(v.begin(), v.end(), 5);
    if (it != v.end()) cout << "找到 5，索引=" << (it - v.begin()) << endl;

    // count_if：统计大于 5 的元素
    int cnt = count_if(v.begin(), v.end(), [](int x){ return x > 5; });
    cout << "大于 5 的元素有 " << cnt << " 个" << endl;

    // transform：每个元素乘以 2（存回 v）
    transform(v.begin(), v.end(), v.begin(), [](int x){ return x * 2; });
    cout << "每个×2: ";
    for (int x : v) cout << x << " ";
    cout << endl;

    // ─── 2. list（双向链表，插入删除 O(1)）────────────────────────
    cout << "\n=== list ===" << endl;
    list<int> lst = {1, 2, 3, 4, 5};
    lst.push_front(0);    // 头部插入
    lst.push_back(6);     // 尾部插入

    auto lit = lst.begin();
    advance(lit, 3);       // 迭代器前进 3 步
    lst.insert(lit, 99);   // 在 lit 前插入 99

    cout << "list: ";
    for (int x : lst) cout << x << " ";
    cout << endl;

    lst.remove(99);        // 删除所有值为 99 的元素
    lst.sort();            // list 有自己的 sort
    cout << "排序后: ";
    for (int x : lst) cout << x << " ";
    cout << endl;

    // ─── 3. map（有序键值对，红黑树）─────────────────────────────
    cout << "\n=== map ===" << endl;
    map<string, int> scores;
    scores["张三"] = 85;
    scores["李四"] = 92;
    scores["王五"] = 78;
    scores.insert({"赵六", 95});
    scores.emplace("孙七", 88);  // emplace 直接构造，效率更高

    print_map(scores, "成绩（自动按键排序）");

    // 查找
    auto mit = scores.find("李四");
    if (mit != scores.end())
        cout << "找到李四：" << mit->second << endl;

    // 安全访问（不存在时不插入新键）
    if (scores.count("王八") == 0)
        cout << "王八不在 map 中" << endl;

    // ─── 4. unordered_map（哈希表，O(1) 平均）────────────────────
    cout << "\n=== unordered_map ===" << endl;
    unordered_map<string, int> word_count;
    string text = "apple banana apple cherry banana apple mango cherry";
    istringstream iss(text);
    string word;
    while (iss >> word) word_count[word]++;

    cout << "单词频率：" << endl;
    for (const auto& [w, c] : word_count)
        cout << "  " << w << ": " << c << endl;

    // 按频率降序排序 -> 先转 vector<pair>
    vector<pair<string, int>> wv(word_count.begin(), word_count.end());
    sort(wv.begin(), wv.end(), [](const auto& a, const auto& b){
        return a.second > b.second;  // 按频率降序
    });
    cout << "按频率排序：" << endl;
    for (const auto& [w, c] : wv)
        cout << "  " << w << ": " << c << endl;

    // ─── 5. set（有序不重复集合）─────────────────────────────────
    cout << "\n=== set ===" << endl;
    set<int> s = {3, 1, 4, 1, 5, 9, 2, 6, 5};  // 自动去重 & 排序
    cout << "set: ";
    for (int x : s) cout << x << " ";
    cout << endl;
    cout << "是否包含 4？" << (s.count(4) ? "是" : "否") << endl;

    // set 的交集
    set<int> s2 = {2, 4, 6, 8};
    vector<int> intersection;
    set_intersection(s.begin(), s.end(), s2.begin(), s2.end(),
                     back_inserter(intersection));
    cout << "与 {2,4,6,8} 的交集：";
    for (int x : intersection) cout << x << " ";
    cout << endl;

    // ─── 6. stack（LIFO，后进先出）───────────────────────────────
    cout << "\n=== stack（用括号匹配验证）===" << endl;
    auto is_balanced = [](const string& expr) -> bool {
        stack<char> stk;
        for (char c : expr) {
            if (c == '(' || c == '[' || c == '{') {
                stk.push(c);
            } else if (c == ')' || c == ']' || c == '}') {
                if (stk.empty()) return false;
                char top = stk.top(); stk.pop();
                if ((c == ')' && top != '(') ||
                    (c == ']' && top != '[') ||
                    (c == '}' && top != '{')) return false;
            }
        }
        return stk.empty();
    };

    vector<string> exprs = {"(a+b)*[c-d]/{e}", "((()]", "{[()]}"};
    for (const string& e : exprs)
        cout << "\"" << e << "\" -> " << (is_balanced(e) ? "✓ 匹配" : "✗ 不匹配") << endl;

    // ─── 7. queue（FIFO，先进先出）───────────────────────────────
    cout << "\n=== queue ===" << endl;
    queue<string> tasks;
    tasks.push("任务A");
    tasks.push("任务B");
    tasks.push("任务C");
    cout << "处理任务（先进先出）：" << endl;
    while (!tasks.empty()) {
        cout << "  处理：" << tasks.front() << endl;
        tasks.pop();
    }

    // ─── 8. priority_queue（优先队列，堆）────────────────────────
    cout << "\n=== priority_queue ===" << endl;
    priority_queue<int> maxPQ;                           // 默认最大堆
    priority_queue<int, vector<int>, greater<int>> minPQ;  // 最小堆

    for (int x : {4, 1, 7, 3, 9, 2}) {
        maxPQ.push(x);
        minPQ.push(x);
    }

    cout << "最大堆出队：";
    while (!maxPQ.empty()) { cout << maxPQ.top() << " "; maxPQ.pop(); }
    cout << endl;

    cout << "最小堆出队：";
    while (!minPQ.empty()) { cout << minPQ.top() << " "; minPQ.pop(); }
    cout << endl;

    // ─── 9. 算法合集 ──────────────────────────────────────────────
    cout << "\n=== 更多算法 ===" << endl;
    vector<int> data = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    // 过滤偶数 (copy_if)
    vector<int> evens;
    copy_if(data.begin(), data.end(), back_inserter(evens),
            [](int x){ return x % 2 == 0; });
    cout << "偶数：";
    for (int x : evens) cout << x << " ";
    cout << endl;

    // 二分查找（要求已排序）
    bool found = binary_search(data.begin(), data.end(), 7);
    cout << "binary_search(7): " << (found ? "找到" : "未找到") << endl;

    // partial_sort：只排前 4 个最小
    vector<int> ps = {5, 2, 8, 1, 9, 3, 7};
    partial_sort(ps.begin(), ps.begin() + 4, ps.end());
    cout << "partial_sort 前4小：";
    for (int x : ps) cout << x << " ";
    cout << endl;

    // unique：去重（先排序！）
    vector<int> dup = {1, 3, 3, 5, 5, 5, 7};
    auto last = unique(dup.begin(), dup.end());
    dup.erase(last, dup.end());
    cout << "unique 去重后：";
    for (int x : dup) cout << x << " ";
    cout << endl;

    return 0;
}
