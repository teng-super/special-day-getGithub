/**
 * 08_cpp20_features.cpp
 * C++20 新特性
 * 演示：Concepts、Ranges、协程（coroutine）简介、
 *       span、std::format（若支持）、三路比较（<=>）、
 *       consteval / constinit
 */

#include <iostream>
#include <concepts>
#include <ranges>
#include <span>
#include <vector>
#include <list>
#include <string>
#include <numeric>
#include <algorithm>
#include <compare>
using namespace std;

// ============================================================
// 一、Concepts（概念）：对模板参数施加约束
// ============================================================

// 使用内置 concept
template <integral T>
T gcd(T a, T b) {
    while (b) { a %= b; swap(a, b); }
    return a;
}

// 自定义 concept
template <typename T>
concept Printable = requires(T t) {
    { cout << t } -> same_as<ostream&>;
};

template <Printable T>
void printValue(const T& v) {
    cout << v << endl;
}

// concept 约束：可加法
template <typename T>
concept Addable = requires(T a, T b) {
    { a + b } -> convertible_to<T>;
};

template <Addable T>
T addTwo(T a, T b) { return a + b; }

// ============================================================
// 二、Ranges（范围库）
// ============================================================
void rangesDemo() {
    cout << "\n=== Ranges ===" << endl;
    vector<int> v = {5, 3, 8, 1, 9, 2, 7, 4, 6};

    // 视图：过滤 + 变换（惰性求值，不修改原容器）
    auto even_doubled = v
        | views::filter([](int x){ return x % 2 == 0; })
        | views::transform([](int x){ return x * 2; });

    cout << "偶数×2: ";
    for (int n : even_doubled) cout << n << " ";
    cout << endl;

    // ranges::sort
    ranges::sort(v);
    cout << "排序后: ";
    for (int n : v) cout << n << " ";
    cout << endl;

    // ranges::reverse_view
    cout << "反转视图: ";
    for (int n : v | views::reverse) cout << n << " ";
    cout << endl;

    // views::take / views::drop
    cout << "前3个: ";
    for (int n : v | views::take(3)) cout << n << " ";
    cout << endl;

    cout << "跳过3个: ";
    for (int n : v | views::drop(3)) cout << n << " ";
    cout << endl;

    // views::iota：生成整数序列
    cout << "iota(1,6): ";
    for (int n : views::iota(1, 6)) cout << n << " ";
    cout << endl;

    // ranges::count_if
    int evens = ranges::count_if(v, [](int x){ return x % 2 == 0; });
    cout << "偶数个数: " << evens << endl;
}

// ============================================================
// 三、span：非拥有视图
// ============================================================
void sumWithSpan(span<const int> s) {
    int total = 0;
    for (int v : s) total += v;
    cout << "span 求和: " << total << endl;
}

// ============================================================
// 四、三路比较运算符（<=>）
// ============================================================
struct Point {
    int x, y;
    // 默认生成所有比较运算符
    auto operator<=>(const Point&) const = default;
};

// 自定义三路比较
struct Version {
    int major, minor, patch;
    auto operator<=>(const Version& o) const {
        if (auto c = major <=> o.major; c != 0) return c;
        if (auto c = minor <=> o.minor; c != 0) return c;
        return patch <=> o.patch;
    }
    bool operator==(const Version&) const = default;
};

// ============================================================
// 五、consteval（编译期函数）& constinit（编译期初始化变量）
// ============================================================
consteval int compiletimeSquare(int n) { return n * n; }

constinit int g_val = 100;   // 保证在编译期初始化，但运行时可修改

// ============================================================
int main() {
    // ---------- Concepts ----------
    cout << "=== Concepts ===" << endl;
    cout << "gcd(12, 8) = " << gcd(12, 8) << endl;
    cout << "gcd(100, 75) = " << gcd(100, 75) << endl;
    printValue(42);
    printValue(string("C++20"));
    cout << "addTwo(3, 4) = "       << addTwo(3, 4)       << endl;
    cout << "addTwo(1.1, 2.2) = "   << addTwo(1.1, 2.2)   << endl;

    rangesDemo();

    // ---------- span ----------
    cout << "\n=== span ===" << endl;
    int arr[] = {1, 2, 3, 4, 5};
    sumWithSpan(arr);
    vector<int> vec = {10, 20, 30};
    sumWithSpan(vec);
    // 子span
    span<int> sp(arr, 5);
    span<int> sub = sp.subspan(1, 3);   // {2,3,4}
    cout << "子span: ";
    for (int v : sub) cout << v << " ";
    cout << endl;

    // ---------- 三路比较 ----------
    cout << "\n=== 三路比较（<=>）===" << endl;
    Point p1{1, 2}, p2{1, 3}, p3{1, 2};
    cout << "p1<p2: " << boolalpha << (p1 < p2) << endl;
    cout << "p1==p3: " << (p1 == p3) << endl;

    Version v1{1,2,3}, v2{1,3,0}, v3{1,2,3};
    cout << "v1<v2: "  << (v1 < v2) << endl;
    cout << "v1==v3: " << (v1 == v3) << endl;

    // ---------- consteval ----------
    cout << "\n=== consteval ===" << endl;
    constexpr int sq = compiletimeSquare(7);   // 编译期计算
    cout << "compiletimeSquare(7) = " << sq << endl;

    // ---------- constinit ----------
    cout << "\n=== constinit ===" << endl;
    cout << "g_val = " << g_val << endl;
    g_val = 200;   // 运行时可修改
    cout << "修改后 g_val = " << g_val << endl;

    return 0;
}
