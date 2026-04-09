/**
 * 05_variadic_templates.cpp
 * C++11 可变参数模板（Variadic Templates）
 * 演示：参数包展开、递归展开、fold expression（C++17）、
 *       完美转发构造、tuple 实现原理简介
 */

#include <iostream>
#include <string>
#include <tuple>
#include <utility>
using namespace std;

// ---------- 递归展开参数包：打印任意数量任意类型参数 ----------
// 终止重载（0 个参数时调用）
void printAll() { cout << endl; }

template <typename First, typename... Rest>
void printAll(const First& first, const Rest&... rest) {
    cout << first << " ";
    printAll(rest...);   // 递归展开
}

// ---------- C++17 fold expression（折叠表达式） ----------
// 求所有参数之和
template <typename... Args>
auto sum(Args... args) {
    return (args + ...);   // 一元右折叠
}

// 所有参数之积
template <typename... Args>
auto product(Args... args) {
    return (args * ...);
}

// 打印（使用折叠 + 逗号运算符）
template <typename... Args>
void printFold(const Args&... args) {
    ((cout << args << " "), ...);
    cout << endl;
}

// ---------- 统计参数个数 ----------
template <typename... Args>
constexpr size_t argCount(Args&&...) { return sizeof...(Args); }

// ---------- 转发参数到另一个函数（完美转发） ----------
template <typename Func, typename... Args>
auto invoke(Func&& f, Args&&... args) {
    return forward<Func>(f)(forward<Args>(args)...);
}

// ---------- 简易 tuple-like 类（演示可变参数继承） ----------
// 通过递归继承实现
template <typename... Types>
struct TypeList {};

// ---------- 使用 std::tuple ----------
void tupleDemo() {
    cout << "\n=== std::tuple ===" << endl;

    auto t = make_tuple(42, 3.14, string("hello"), true);
    cout << "get<0>=" << get<0>(t) << endl;
    cout << "get<1>=" << get<1>(t) << endl;
    cout << "get<2>=" << get<2>(t) << endl;
    cout << "get<3>=" << boolalpha << get<3>(t) << endl;
    cout << "tuple_size=" << tuple_size<decltype(t)>::value << endl;

    // C++17 结构化绑定解包 tuple
    auto [a, b, c, d] = t;
    cout << "结构化绑定: " << a << ", " << b << ", " << c << ", " << d << endl;

    // tie 解包
    int    i; double f; string s; bool bl;
    tie(i, f, s, bl) = t;
    cout << "tie 解包: i=" << i << ", f=" << f << ", s=" << s << endl;

    // tuple_cat 拼接
    auto t2 = make_tuple(100, 200);
    auto t3 = tuple_cat(t, t2);
    cout << "拼接后大小: " << tuple_size<decltype(t3)>::value << endl;
}

// ======================================================
int main() {
    cout << "=== printAll（递归展开） ===" << endl;
    printAll(1, 2.5, "hello", 'A', true);
    printAll("C++", 11, 14, 17, 20);

    cout << "\n=== fold expression（C++17） ===" << endl;
    cout << "sum(1,2,3,4,5) = "       << sum(1, 2, 3, 4, 5)       << endl;
    cout << "sum(1.1,2.2,3.3) = "     << sum(1.1, 2.2, 3.3)       << endl;
    cout << "product(1,2,3,4,5) = "   << product(1, 2, 3, 4, 5)   << endl;
    printFold(10, 20, 30, "world");

    cout << "\n=== sizeof... ===" << endl;
    cout << "argCount(1,2,3) = " << argCount(1, 2, 3) << endl;
    cout << "argCount() = "      << argCount()         << endl;

    cout << "\n=== invoke（完美转发） ===" << endl;
    auto result = invoke([](int a, int b){ return a + b; }, 10, 20);
    cout << "invoke 结果: " << result << endl;

    tupleDemo();

    return 0;
}
