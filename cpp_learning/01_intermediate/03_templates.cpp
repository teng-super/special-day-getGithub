/**
 * 【C++ 中级 - 第3课：模板（Templates）】
 *
 * 知识点：
 * 1. 函数模板（Function Template）
 * 2. 类模板（Class Template）
 * 3. 模板特化（Template Specialization）
 * 4. 非类型模板参数
 * 5. 模板与继承的结合
 * 6. C++11：变参模板（Variadic Templates）基础
 * 7. C++11：模板别名（using alias）
 *
 * 练习：
 *   - 实现一个通用的 Stack<T> 栈类
 *   - 实现一个通用的 Pair<T1, T2> 类
 */

#include <iostream>
#include <vector>
#include <stdexcept>
#include <string>
#include <sstream>

using namespace std;

// ══════════════════════════════════════════════════════════════════
// 1. 函数模板
// ══════════════════════════════════════════════════════════════════

// 通用最大值函数（替代宏 #define MAX）
template <typename T>
T my_max(T a, T b) {
    return a > b ? a : b;
}

// 多参数类型
template <typename T, typename U>
auto add(T a, U b) -> decltype(a + b) {  // C++11 尾返回类型
    return a + b;
}

// 通用 swap
template <typename T>
void my_swap(T& a, T& b) {
    T temp = a;
    a = b;
    b = temp;
}

// 通用打印函数：打印任意可迭代容器
template <typename Container>
void print_container(const Container& c, const string& label = "") {
    if (!label.empty()) cout << label << ": ";
    cout << "[";
    bool first = true;
    for (const auto& x : c) {
        if (!first) cout << ", ";
        cout << x;
        first = false;
    }
    cout << "]" << endl;
}

// ══════════════════════════════════════════════════════════════════
// 2. 类模板：通用 Stack<T>
// ══════════════════════════════════════════════════════════════════
template <typename T>
class Stack {
private:
    vector<T> data;
    size_t    cap;    // 最大容量

public:
    // 默认容量 100
    explicit Stack(size_t capacity = 100) : cap(capacity) {}

    void push(const T& val) {
        if (data.size() >= cap)
            throw overflow_error("Stack overflow!");
        data.push_back(val);
    }

    T pop() {
        if (empty())
            throw underflow_error("Stack underflow!");
        T top = data.back();
        data.pop_back();
        return top;
    }

    const T& top() const {
        if (empty())
            throw underflow_error("Stack is empty!");
        return data.back();
    }

    bool   empty() const { return data.empty(); }
    size_t size()  const { return data.size(); }

    // 转为字符串表示
    string to_str() const {
        ostringstream oss;
        oss << "Stack[";
        for (size_t i = 0; i < data.size(); i++) {
            if (i) oss << ", ";
            oss << data[i];
        }
        oss << "] (top=" << (empty() ? "空" : to_string_helper(data.back())) << ")";
        return oss.str();
    }

private:
    // 辅助：用 stringstream 转换任意类型为 string
    static string to_string_helper(const T& val) {
        ostringstream oss;
        oss << val;
        return oss.str();
    }
};

// ══════════════════════════════════════════════════════════════════
// 3. 类模板：通用 Pair<T1, T2>
// ══════════════════════════════════════════════════════════════════
template <typename T1, typename T2>
class MyPair {
public:
    T1 first;
    T2 second;

    MyPair(T1 f, T2 s) : first(f), second(s) {}

    // 运算符重载
    bool operator==(const MyPair& other) const {
        return first == other.first && second == other.second;
    }

    friend ostream& operator<<(ostream& os, const MyPair& p) {
        return os << "(" << p.first << ", " << p.second << ")";
    }
};

// 工厂函数（类型自动推断）
template <typename T1, typename T2>
MyPair<T1, T2> make_my_pair(T1 a, T2 b) {
    return MyPair<T1, T2>(a, b);
}

// ══════════════════════════════════════════════════════════════════
// 4. 模板特化
// ══════════════════════════════════════════════════════════════════
// 通用版本
template <typename T>
string describe(T val) {
    return "值: " + to_string(val);
}

// 对 string 类型进行全特化
template <>
string describe<string>(string val) {
    return "字符串: \"" + val + "\" (长度=" + to_string(val.length()) + ")";
}

// 对 bool 类型进行全特化
template <>
string describe<bool>(bool val) {
    return string("布尔值: ") + (val ? "true" : "false");
}

// ══════════════════════════════════════════════════════════════════
// 5. 非类型模板参数：固定大小数组
// ══════════════════════════════════════════════════════════════════
template <typename T, int N>
class FixedArray {
private:
    T data[N];
    int count = 0;

public:
    FixedArray() : count(0) {}

    void add(const T& val) {
        if (count < N) data[count++] = val;
    }

    void print() const {
        cout << "FixedArray<" << N << ">: [";
        for (int i = 0; i < count; i++) {
            if (i) cout << ", ";
            cout << data[i];
        }
        cout << "]" << endl;
    }

    int  size()     const { return count;    }
    int  capacity() const { return N;        }
    bool full()     const { return count >= N; }
};

// ══════════════════════════════════════════════════════════════════
// 6. 变参模板（Variadic Templates, C++11）
// ══════════════════════════════════════════════════════════════════
// 递归终止
void print_all() {
    cout << endl;
}

// 展开参数包
template <typename T, typename... Args>
void print_all(T first, Args... rest) {
    cout << first;
    if (sizeof...(rest) > 0) cout << ", ";
    print_all(rest...);  // 递归展开
}

// 变参求和
template <typename T>
T sum(T val) { return val; }

template <typename T, typename... Args>
T sum(T first, Args... rest) {
    return first + sum(rest...);
}

// ══════════════════════════════════════════════════════════════════
// 7. 模板别名（C++11 using）
// ══════════════════════════════════════════════════════════════════
template <typename T>
using Vec = vector<T>;  // 简化写法

using IntPair = MyPair<int, int>;

// ══════════════════════════════════════════════════════════════════
// main
// ══════════════════════════════════════════════════════════════════
int main() {
    // ─── 函数模板 ─────────────────────────────────────────────────
    cout << "=== 函数模板 ===" << endl;
    cout << "my_max(3, 7)       = " << my_max(3, 7)        << endl;
    cout << "my_max(3.14, 2.72) = " << my_max(3.14, 2.72)  << endl;
    cout << "my_max<string>(\"apple\",\"banana\") = "
         << my_max<string>("apple", "banana") << endl;

    int pa = 10, pb = 20;
    my_swap(pa, pb);
    cout << "my_swap 后：pa=" << pa << " pb=" << pb << endl;

    Vec<int> vi = {3, 1, 4, 1, 5, 9};
    print_container(vi, "int vector");

    Vec<string> vs = {"C++", "Java", "Python"};
    print_container(vs, "string vector");

    // ─── Stack<T> ─────────────────────────────────────────────────
    cout << "\n=== Stack<T> ===" << endl;
    Stack<int> stk;
    for (int x : {1, 2, 3, 4, 5}) stk.push(x);
    cout << stk.to_str() << endl;

    while (!stk.empty()) cout << "pop: " << stk.pop() << endl;

    try {
        stk.pop();  // 触发异常
    } catch (const underflow_error& e) {
        cout << "捕获异常：" << e.what() << endl;
    }

    Stack<string> sstk;
    sstk.push("Hello");
    sstk.push("World");
    cout << sstk.to_str() << endl;

    // ─── Pair<T1, T2> ─────────────────────────────────────────────
    cout << "\n=== MyPair<T1, T2> ===" << endl;
    auto p1 = make_my_pair(42, 3.14);
    auto p2 = make_my_pair(string("name"), string("Alice"));
    cout << "p1 = " << p1 << endl;
    cout << "p2 = " << p2 << endl;
    IntPair ip(1, 2);
    cout << "IntPair ip = " << ip << endl;

    // ─── 模板特化 ─────────────────────────────────────────────────
    cout << "\n=== 模板特化 ===" << endl;
    cout << describe(42)            << endl;
    cout << describe(3.14)          << endl;
    cout << describe(string("Hi"))  << endl;
    cout << describe(true)          << endl;

    // ─── 非类型模板参数 ───────────────────────────────────────────
    cout << "\n=== 非类型模板参数 ===" << endl;
    FixedArray<int, 5> fa;
    for (int x : {10, 20, 30}) fa.add(x);
    fa.print();
    cout << "容量=" << fa.capacity() << " 已用=" << fa.size() << endl;

    // ─── 变参模板 ─────────────────────────────────────────────────
    cout << "\n=== 变参模板 ===" << endl;
    cout << "print_all(1, 2.5, \"hello\", 'A'): ";
    print_all(1, 2.5, string("hello"), 'A');

    cout << "sum(1,2,3,4,5) = " << sum(1, 2, 3, 4, 5)       << endl;
    cout << "sum(1.1,2.2,3.3) = " << sum(1.1, 2.2, 3.3)     << endl;

    return 0;
}
