/**
 * 02_lambda.cpp
 * C++11 Lambda 表达式
 * 演示：语法、捕获方式、泛型 lambda（C++14）、即时调用、
 *       在 STL 算法中使用 lambda
 */

#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>
#include <numeric>
using namespace std;

// ---------- 接受 std::function 的高阶函数 ----------
void applyTwice(function<int(int)> f, int x) {
    cout << "f(x) = " << f(x) << "，f(f(x)) = " << f(f(x)) << endl;
}

int main() {
    // ---------- 基本语法 ----------
    cout << "=== 基本 lambda ===" << endl;
    auto greet = []() { cout << "Hello from lambda!" << endl; };
    greet();

    auto add = [](int a, int b) -> int { return a + b; };
    cout << "add(3,4) = " << add(3, 4) << endl;

    // ---------- 值捕获 & 引用捕获 ----------
    cout << "\n=== 捕获方式 ===" << endl;
    int x = 10, y = 20;
    // [=]  按值捕获所有外部变量
    auto byValue = [=]() { cout << "值捕获 x=" << x << ", y=" << y << endl; };
    // [&]  按引用捕获所有外部变量
    auto byRef   = [&]() { x += 5; cout << "引用捕获后 x=" << x << endl; };
    byValue();
    byRef();
    cout << "调用后 x=" << x << endl;

    // 捕获特定变量
    int factor = 3;
    auto multiply = [factor](int n) { return n * factor; };
    cout << "multiply(7) = " << multiply(7) << endl;

    // ---------- mutable：允许修改值捕获的副本 ----------
    cout << "\n=== mutable ===" << endl;
    int counter = 0;
    auto inc = [counter]() mutable {
        counter++;
        cout << "内部 counter=" << counter << endl;
    };
    inc(); inc(); inc();
    cout << "外部 counter 未变: " << counter << endl;

    // ---------- 高阶函数 ----------
    cout << "\n=== 高阶函数 ===" << endl;
    applyTwice([](int n){ return n * 2; }, 3);
    applyTwice([](int n){ return n + 10; }, 5);

    // ---------- STL 算法中的 lambda ----------
    cout << "\n=== STL 算法 ===" << endl;
    vector<int> nums = {5, 3, 8, 1, 9, 2, 7, 4, 6};

    // sort 降序
    sort(nums.begin(), nums.end(), [](int a, int b){ return a > b; });
    cout << "降序: ";
    for (int v : nums) cout << v << " ";
    cout << endl;

    // find_if
    auto it = find_if(nums.begin(), nums.end(), [](int v){ return v < 4; });
    if (it != nums.end()) cout << "第一个 <4 的元素: " << *it << endl;

    // count_if
    int evenCount = count_if(nums.begin(), nums.end(), [](int v){ return v % 2 == 0; });
    cout << "偶数个数: " << evenCount << endl;

    // transform
    vector<int> doubled(nums.size());
    transform(nums.begin(), nums.end(), doubled.begin(), [](int v){ return v * 2; });
    cout << "乘以2: ";
    for (int v : doubled) cout << v << " ";
    cout << endl;

    // accumulate（求和）
    int sum = accumulate(nums.begin(), nums.end(), 0, [](int acc, int v){ return acc + v; });
    cout << "求和: " << sum << endl;

    // ---------- 泛型 lambda（C++14） ----------
    cout << "\n=== 泛型 lambda (C++14) ===" << endl;
    auto print = [](const auto& v) { cout << v << " "; };
    print(42);
    print(3.14);
    print("hello");
    cout << endl;

    auto genericAdd = [](auto a, auto b) { return a + b; };
    cout << "genericAdd(1, 2)     = " << genericAdd(1, 2)     << endl;
    cout << "genericAdd(1.5, 2.3) = " << genericAdd(1.5, 2.3) << endl;

    // ---------- 即时调用 lambda (IIFE) ----------
    cout << "\n=== IIFE ===" << endl;
    int result = [](int a, int b){ return a * a + b * b; }(3, 4);
    cout << "3² + 4² = " << result << endl;

    // ---------- 返回 lambda 的函数 ----------
    cout << "\n=== 返回 lambda ===" << endl;
    auto makeAdder = [](int base) {
        return [base](int n) { return base + n; };
    };
    auto add5  = makeAdder(5);
    auto add10 = makeAdder(10);
    cout << "add5(3)  = " << add5(3)  << endl;
    cout << "add10(3) = " << add10(3) << endl;

    return 0;
}
