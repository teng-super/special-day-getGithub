/**
 * 08_templates.cpp
 * C++ 入门：模板（泛型编程）
 * 演示函数模板、类模板、模板特化
 */

#include <iostream>
#include <string>
#include <cstring>
using namespace std;

// ---------- 函数模板 ----------
template <typename T>
T maxValue(T a, T b) {
    return (a > b) ? a : b;
}

// 函数模板：交换
template <typename T>
void mySwap(T& a, T& b) {
    T tmp = a;
    a = b;
    b = tmp;
}

// ---------- 类模板：简单栈 ----------
template <typename T, int MaxSize = 10>
class Stack {
private:
    T   data[MaxSize];
    int top;

public:
    Stack() : top(-1) {}

    bool push(const T& val) {
        if (top >= MaxSize - 1) { cerr << "栈满" << endl; return false; }
        data[++top] = val;
        return true;
    }

    bool pop(T& val) {
        if (top < 0) { cerr << "栈空" << endl; return false; }
        val = data[top--];
        return true;
    }

    T peek() const {
        if (top < 0) { cerr << "栈空" << endl; return T{}; }
        return data[top];
    }

    bool empty() const { return top < 0; }
    int  size()  const { return top + 1; }
};

// ---------- 模板特化：针对 const char* 的 maxValue ----------
template <>
const char* maxValue<const char*>(const char* a, const char* b) {
    return (strcmp(a, b) > 0) ? a : b;
}

// ---------- 模板函数：打印任意类型的数组 ----------
template <typename T>
void printArray(const T* arr, int n) {
    cout << "[";
    for (int i = 0; i < n; i++) {
        cout << arr[i];
        if (i < n - 1) cout << ", ";
    }
    cout << "]" << endl;
}

// ======================================================
int main() {
    cout << "=== 函数模板 ===" << endl;
    cout << "maxValue(3, 7)       = " << maxValue(3, 7)       << endl;
    cout << "maxValue(3.14, 2.72) = " << maxValue(3.14, 2.72) << endl;
    cout << "maxValue('a', 'z')   = " << maxValue('a', 'z')   << endl;
    string s1 = "apple", s2 = "banana";
    cout << "maxValue(\"apple\",\"banana\") = " << maxValue(s1, s2) << endl;

    cout << "\n=== 函数模板：交换 ===" << endl;
    int a = 10, b = 20;
    mySwap(a, b);
    cout << "交换后: a=" << a << ", b=" << b << endl;

    cout << "\n=== 类模板：Stack ===" << endl;
    Stack<int> intStack;
    intStack.push(1);
    intStack.push(2);
    intStack.push(3);
    cout << "栈顶: " << intStack.peek() << "，大小: " << intStack.size() << endl;
    int val;
    intStack.pop(val);
    cout << "弹出: " << val << "，剩余大小: " << intStack.size() << endl;

    Stack<string, 5> strStack;
    strStack.push("hello");
    strStack.push("world");
    cout << "字符串栈顶: " << strStack.peek() << endl;

    cout << "\n=== 模板特化 ===" << endl;
    cout << "maxValue(\"abc\",\"xyz\") = "
         << maxValue<const char*>("abc", "xyz") << endl;

    cout << "\n=== 打印数组 ===" << endl;
    int    iarr[] = {1, 2, 3, 4, 5};
    double darr[] = {1.1, 2.2, 3.3};
    printArray(iarr, 5);
    printArray(darr, 3);

    return 0;
}
