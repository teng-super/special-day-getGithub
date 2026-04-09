/**
 * 04_functions.cpp
 * C++ 入门：函数
 * 演示函数声明/定义、默认参数、函数重载、内联函数、递归
 */

#include <iostream>
using namespace std;

// ---------- 函数声明（前置声明） ----------
int add(int a, int b);
double power(double base, int exp);

// ---------- 默认参数 ----------
void greet(const string& name, const string& greeting = "你好");

// ---------- 函数重载：同名不同参 ----------
int    maxVal(int a, int b);
double maxVal(double a, double b);

// ---------- 内联函数（适合短小频繁调用的函数） ----------
inline int square(int x) { return x * x; }

// ---------- 递归：计算阶乘 ----------
long long factorial(int n);

// ======================================================
int main() {
    cout << "=== 普通函数 ===" << endl;
    cout << "add(3, 4) = " << add(3, 4) << endl;
    cout << "2^10 = " << power(2.0, 10) << endl;

    cout << "\n=== 默认参数 ===" << endl;
    greet("Alice");
    greet("Bob", "早上好");

    cout << "\n=== 函数重载 ===" << endl;
    cout << "maxVal(3, 7)     = " << maxVal(3, 7)     << endl;
    cout << "maxVal(3.5, 2.1) = " << maxVal(3.5, 2.1) << endl;

    cout << "\n=== 内联函数 ===" << endl;
    cout << "square(5) = " << square(5) << endl;

    cout << "\n=== 递归 ===" << endl;
    for (int i = 0; i <= 10; i++) {
        cout << i << "! = " << factorial(i) << endl;
    }

    // ---------- lambda 函数简介（详见 advanced 目录） ----------
    cout << "\n=== lambda 函数简介 ===" << endl;
    auto multiply = [](int x, int y) { return x * y; };
    cout << "multiply(6, 7) = " << multiply(6, 7) << endl;

    return 0;
}

// ---------- 函数定义 ----------
int add(int a, int b) {
    return a + b;
}

double power(double base, int exp) {
    double result = 1.0;
    for (int i = 0; i < exp; i++) result *= base;
    return result;
}

void greet(const string& name, const string& greeting) {
    cout << greeting << ", " << name << "!" << endl;
}

int maxVal(int a, int b) {
    return (a > b) ? a : b;
}

double maxVal(double a, double b) {
    return (a > b) ? a : b;
}

long long factorial(int n) {
    if (n <= 1) return 1;          // 递归终止条件
    return n * factorial(n - 1);   // 递归调用
}
