/**
 * 10_practice_projects.cpp
 * C++ 综合练习项目
 * 项目一：学生成绩管理系统（CRUD + 排序 + 统计）
 * 项目二：简单计算器（栈实现中缀表达式求值）
 * 项目三：链表实现（手写单向链表）
 */

#include <iostream>
#include <vector>
#include <map>
#include <stack>
#include <string>
#include <sstream>
#include <algorithm>
#include <numeric>
#include <iomanip>
#include <stdexcept>
using namespace std;

// ============================================================
// 项目一：学生成绩管理
// ============================================================
struct Student {
    int    id;
    string name;
    map<string, double> scores;  // 科目 -> 成绩

    double average() const {
        if (scores.empty()) return 0.0;
        double sum = 0;
        for (const auto& [_, s] : scores) sum += s;
        return sum / scores.size();
    }
};

class ScoreManager {
    vector<Student> students;
    int nextId = 1;
public:
    void addStudent(const string& name) {
        students.push_back({nextId++, name, {}});
        cout << "添加学生: " << name << " (ID=" << students.back().id << ")" << endl;
    }

    Student* findById(int id) {
        for (auto& s : students)
            if (s.id == id) return &s;
        return nullptr;
    }

    void addScore(int id, const string& subject, double score) {
        if (auto* s = findById(id)) {
            s->scores[subject] = score;
        } else {
            cout << "未找到 ID=" << id << endl;
        }
    }

    void printAll() const {
        cout << left << setw(4) << "ID"
             << setw(10) << "姓名"
             << setw(8) << "平均分" << endl;
        cout << string(24, '-') << endl;
        for (const auto& s : students) {
            cout << left << setw(4) << s.id
                 << setw(10) << s.name
                 << fixed << setprecision(2) << s.average() << endl;
        }
    }

    void printRanking() {
        vector<Student> sorted = students;
        sort(sorted.begin(), sorted.end(),
             [](const Student& a, const Student& b){ return a.average() > b.average(); });
        cout << "\n排名:" << endl;
        for (int i = 0; i < (int)sorted.size(); i++) {
            cout << i+1 << ". " << sorted[i].name
                 << " - " << fixed << setprecision(2) << sorted[i].average() << endl;
        }
    }

    void statistics() const {
        if (students.empty()) return;
        vector<double> avgs;
        for (const auto& s : students) avgs.push_back(s.average());
        double total = accumulate(avgs.begin(), avgs.end(), 0.0);
        double classAvg = total / avgs.size();
        double maxAvg = *max_element(avgs.begin(), avgs.end());
        double minAvg = *min_element(avgs.begin(), avgs.end());
        cout << "\n统计: 班级均分=" << fixed << setprecision(2) << classAvg
             << ", 最高=" << maxAvg << ", 最低=" << minAvg << endl;
    }
};

// ============================================================
// 项目二：栈实现的简单计算器（仅支持 + - * / 和括号）
// ============================================================
double calculate(const string& expr) {
    stack<double> nums;
    stack<char>   ops;

    auto priority = [](char op) -> int {
        if (op == '+' || op == '-') return 1;
        if (op == '*' || op == '/') return 2;
        return 0;
    };

    auto applyOp = [&]() {
        if (nums.size() < 2 || ops.empty()) throw runtime_error("表达式错误");
        double b = nums.top(); nums.pop();
        double a = nums.top(); nums.pop();
        char   op = ops.top(); ops.pop();
        switch (op) {
            case '+': nums.push(a + b); break;
            case '-': nums.push(a - b); break;
            case '*': nums.push(a * b); break;
            case '/':
                if (b == 0) throw runtime_error("除以零");
                nums.push(a / b); break;
        }
    };

    istringstream iss(expr);
    char c;
    while (iss >> c) {
        if (isdigit(c) || c == '.') {
            iss.putback(c);
            double num; iss >> num;
            nums.push(num);
        } else if (c == '(') {
            ops.push(c);
        } else if (c == ')') {
            while (!ops.empty() && ops.top() != '(') applyOp();
            if (!ops.empty()) ops.pop();  // 弹出 '('
        } else if (c == '+' || c == '-' || c == '*' || c == '/') {
            while (!ops.empty() && priority(ops.top()) >= priority(c)) applyOp();
            ops.push(c);
        }
    }
    while (!ops.empty()) applyOp();
    return nums.top();
}

// ============================================================
// 项目三：手写单向链表
// ============================================================
template <typename T>
class LinkedList {
    struct Node {
        T    val;
        Node* next;
        explicit Node(const T& v) : val(v), next(nullptr) {}
    };
    Node* head;
    int   sz;
public:
    LinkedList() : head(nullptr), sz(0) {}

    ~LinkedList() {
        Node* cur = head;
        while (cur) { Node* nxt = cur->next; delete cur; cur = nxt; }
    }

    void pushFront(const T& v) {
        Node* n = new Node(v);
        n->next = head;
        head = n;
        sz++;
    }

    void pushBack(const T& v) {
        Node* n = new Node(v);
        if (!head) { head = n; sz++; return; }
        Node* cur = head;
        while (cur->next) cur = cur->next;
        cur->next = n;
        sz++;
    }

    bool remove(const T& v) {
        if (!head) return false;
        if (head->val == v) {
            Node* tmp = head;
            head = head->next;
            delete tmp; sz--;
            return true;
        }
        Node* cur = head;
        while (cur->next && cur->next->val != v) cur = cur->next;
        if (cur->next) {
            Node* tmp = cur->next;
            cur->next = tmp->next;
            delete tmp; sz--;
            return true;
        }
        return false;
    }

    void reverse() {
        Node* prev = nullptr;
        Node* cur  = head;
        while (cur) {
            Node* nxt = cur->next;
            cur->next = prev;
            prev = cur;
            cur  = nxt;
        }
        head = prev;
    }

    void print() const {
        Node* cur = head;
        while (cur) {
            cout << cur->val;
            if (cur->next) cout << " -> ";
            cur = cur->next;
        }
        cout << " (size=" << sz << ")" << endl;
    }

    int size() const { return sz; }
};

// ============================================================
int main() {
    cout << "========== 项目一：学生成绩管理 ==========" << endl;
    ScoreManager mgr;
    mgr.addStudent("张三");
    mgr.addStudent("李四");
    mgr.addStudent("王五");
    mgr.addScore(1, "数学", 88);  mgr.addScore(1, "英语", 92); mgr.addScore(1, "物理", 75);
    mgr.addScore(2, "数学", 95);  mgr.addScore(2, "英语", 80); mgr.addScore(2, "物理", 90);
    mgr.addScore(3, "数学", 72);  mgr.addScore(3, "英语", 85); mgr.addScore(3, "物理", 68);
    mgr.printAll();
    mgr.printRanking();
    mgr.statistics();

    cout << "\n========== 项目二：计算器 ==========" << endl;
    vector<string> exprs = {
        "3 + 4 * 2",
        "( 1 + 2 ) * ( 3 + 4 )",
        "10 / 2 + 3 * 4",
        "( 10 - 4 ) / 2 + 1"
    };
    for (const auto& e : exprs) {
        cout << e << " = " << calculate(e) << endl;
    }

    cout << "\n========== 项目三：链表 ==========" << endl;
    LinkedList<int> ll;
    ll.pushBack(1); ll.pushBack(2); ll.pushBack(3);
    ll.pushFront(0);
    ll.pushBack(4);
    cout << "初始: "; ll.print();
    ll.remove(2);
    cout << "删除2: "; ll.print();
    ll.reverse();
    cout << "反转: "; ll.print();

    LinkedList<string> sll;
    sll.pushBack("C"); sll.pushBack("++"); sll.pushBack("20");
    cout << "字符串链表: "; sll.print();

    return 0;
}
