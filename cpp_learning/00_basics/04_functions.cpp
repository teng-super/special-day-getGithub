/**
 * 【C++ 基础 - 第4课：函数】
 *
 * 知识点：
 * 1. 函数声明（前置声明）与定义
 * 2. 值传递 vs 引用传递 vs 指针传递
 * 3. 默认参数
 * 4. 函数重载（Overloading）
 * 5. 内联函数（inline）
 * 6. 递归函数
 * 7. 函数返回多个值（通过引用参数 or pair/tuple）
 *
 * 练习：
 *   - 实现斐波那契数列（递归 vs 迭代对比）
 *   - 实现冒泡排序函数
 */

#include <iostream>
#include <utility>   // pair
#include <tuple>     // tuple（C++11）
#include <vector>
#include <chrono>    // 计时（C++11）

using namespace std;

// ─── 前置声明 ──────────────────────────────────────────────────────
int  add(int a, int b);
void swap_by_ref(int& a, int& b);
void swap_by_ptr(int* a, int* b);

// ─── 1. 基本函数 & 默认参数 ────────────────────────────────────────
// 计算矩形面积，width 有默认值 1（默认参数必须从右向左设置）
double rect_area(double length, double width = 1.0) {
    return length * width;
}

// ─── 2. 函数重载 ───────────────────────────────────────────────────
// 同名函数，参数列表不同
int    max_val(int a, int b)       { return a > b ? a : b; }
double max_val(double a, double b) { return a > b ? a : b; }
int    max_val(int a, int b, int c){ return max_val(max_val(a, b), c); }

// ─── 3. 内联函数（编译器建议展开，避免函数调用开销）────────────────
inline int square(int x) { return x * x; }

// ─── 4. 值传递 vs 引用传递 vs 指针传递 ────────────────────────────
void increment_by_value(int x)  { x++;  }  // 无法修改外部变量
void increment_by_ref(int& x)   { x++;  }  // 通过引用修改
void increment_by_ptr(int* px)  { (*px)++; }  // 通过指针修改

int add(int a, int b) { return a + b; }
void swap_by_ref(int& a, int& b) { int t = a; a = b; b = t; }
void swap_by_ptr(int* a, int* b) { int t = *a; *a = *b; *b = t; }

// ─── 5. 返回多个值 ─────────────────────────────────────────────────
pair<int, int> minmax(const vector<int>& v) {
    int mn = v[0], mx = v[0];
    for (int x : v) {
        if (x < mn) mn = x;
        if (x > mx) mx = x;
    }
    return {mn, mx};  // C++11 统一初始化
}

// 使用 tuple 返回三个值
tuple<double, double, double> stats(const vector<double>& v) {
    double sum = 0, mn = v[0], mx = v[0];
    for (double x : v) {
        sum += x;
        if (x < mn) mn = x;
        if (x > mx) mx = x;
    }
    return make_tuple(sum / v.size(), mn, mx);
}

// ─── 6. 递归：阶乘 ─────────────────────────────────────────────────
long long factorial(int n) {
    if (n <= 1) return 1;          // 基础情形
    return n * factorial(n - 1);  // 递归调用
}

// ─── 7. 斐波那契：递归（慢）vs 迭代（快）──────────────────────────
long long fib_recursive(int n) {
    if (n <= 1) return n;
    return fib_recursive(n - 1) + fib_recursive(n - 2);
}

long long fib_iterative(int n) {
    if (n <= 1) return n;
    long long a = 0, b = 1;
    for (int i = 2; i <= n; i++) {
        long long c = a + b;
        a = b;
        b = c;
    }
    return b;
}

// ─── 8. 冒泡排序 ───────────────────────────────────────────────────
void bubble_sort(vector<int>& arr) {
    int n = arr.size();
    for (int i = 0; i < n - 1; i++) {
        bool swapped = false;
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]);
                swapped = true;
            }
        }
        if (!swapped) break;  // 已排好，提前结束
    }
}

int main() {
    // ─── 测试默认参数 ─────────────────────────────────────────────
    cout << "=== 默认参数 ===" << endl;
    cout << "rect_area(5)     = " << rect_area(5)     << endl;
    cout << "rect_area(5, 3)  = " << rect_area(5, 3)  << endl;

    // ─── 测试函数重载 ─────────────────────────────────────────────
    cout << "\n=== 函数重载 ===" << endl;
    cout << "max_val(3, 7)       = " << max_val(3, 7)       << endl;
    cout << "max_val(2.5, 1.8)   = " << max_val(2.5, 1.8)   << endl;
    cout << "max_val(4, 9, 6)    = " << max_val(4, 9, 6)    << endl;

    // ─── 测试内联函数 ─────────────────────────────────────────────
    cout << "\n=== 内联函数 ===" << endl;
    cout << "square(7)  = " << square(7) << endl;

    // ─── 测试三种传参方式 ─────────────────────────────────────────
    cout << "\n=== 三种传参方式 ===" << endl;
    int v = 10;
    increment_by_value(v);  cout << "值传递后 v = " << v << endl;  // 10
    increment_by_ref(v);    cout << "引用传递后 v = " << v << endl; // 11
    increment_by_ptr(&v);   cout << "指针传递后 v = " << v << endl; // 12

    int p = 3, q = 8;
    cout << "\n交换前：p=" << p << " q=" << q << endl;
    swap_by_ref(p, q);
    cout << "swap_by_ref 后：p=" << p << " q=" << q << endl;

    // ─── 测试 pair 返回 ───────────────────────────────────────────
    cout << "\n=== pair 返回多值 ===" << endl;
    vector<int> data = {5, 2, 8, 1, 9, 3};
    auto [mn, mx] = minmax(data);  // C++17 结构化绑定
    cout << "最小值=" << mn << "，最大值=" << mx << endl;

    // ─── 测试递归阶乘 ─────────────────────────────────────────────
    cout << "\n=== 递归：阶乘 ===" << endl;
    for (int i = 0; i <= 10; i++) {
        cout << i << "! = " << factorial(i) << endl;
    }

    // ─── 斐波那契性能对比 ─────────────────────────────────────────
    cout << "\n=== 斐波那契 递归 vs 迭代 ===" << endl;
    int N = 35;

    auto t1 = chrono::high_resolution_clock::now();
    long long r1 = fib_recursive(N);
    auto t2 = chrono::high_resolution_clock::now();
    long long r2 = fib_iterative(N);
    auto t3 = chrono::high_resolution_clock::now();

    auto us_rec = chrono::duration_cast<chrono::microseconds>(t2 - t1).count();
    auto us_ite = chrono::duration_cast<chrono::microseconds>(t3 - t2).count();

    cout << "fib(" << N << ") = " << r1 << "  递归耗时：" << us_rec << " μs" << endl;
    cout << "fib(" << N << ") = " << r2 << "  迭代耗时：" << us_ite << " μs" << endl;

    // ─── 冒泡排序 ─────────────────────────────────────────────────
    cout << "\n=== 冒泡排序 ===" << endl;
    vector<int> arr = {64, 25, 12, 22, 11};
    cout << "排序前：";
    for (int x : arr) cout << x << " ";
    bubble_sort(arr);
    cout << "\n排序后：";
    for (int x : arr) cout << x << " ";
    cout << endl;

    return 0;
}
