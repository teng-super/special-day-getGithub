/**
 * 07_cpp17_features.cpp
 * C++17 新特性
 * 演示：结构化绑定、if/switch 初始化语句、std::optional、
 *       std::variant、std::any、constexpr if、string_view、
 *       并行算法、std::filesystem（简介）
 */

#include <iostream>
#include <optional>
#include <variant>
#include <any>
#include <string>
#include <string_view>
#include <map>
#include <vector>
#include <algorithm>
#include <numeric>
#include <chrono>
using namespace std;

// ---------- std::optional：可能没有值的返回 ----------
optional<int> safeDiv(int a, int b) {
    if (b == 0) return nullopt;
    return a / b;
}

optional<string> findUser(const map<int,string>& db, int id) {
    auto it = db.find(id);
    if (it == db.end()) return nullopt;
    return it->second;
}

// ---------- std::variant：类型安全的联合体 ----------
using Number = variant<int, double, string>;

void printNumber(const Number& n) {
    visit([](const auto& v){ cout << v << " (" << typeid(v).name() << ")" << endl; }, n);
}

// ---------- constexpr if：编译期分支 ----------
template <typename T>
void describe(const T& val) {
    if constexpr (is_integral_v<T>) {
        cout << "整数: " << val << endl;
    } else if constexpr (is_floating_point_v<T>) {
        cout << "浮点: " << val << endl;
    } else {
        cout << "其他: " << val << endl;
    }
}

// ======================================================
int main() {
    // ---------- 结构化绑定 ----------
    cout << "=== 结构化绑定 ===" << endl;
    pair<string,int> p = {"Alice", 30};
    auto& [name, age] = p;
    cout << name << " 年龄: " << age << endl;

    map<string,int> scores = {{"Bob",90},{"Carol",85}};
    for (const auto& [n, s] : scores) {
        cout << n << ": " << s << endl;
    }

    // 数组结构化绑定
    int arr[3] = {10, 20, 30};
    auto [x, y, z] = arr;
    cout << "x=" << x << ", y=" << y << ", z=" << z << endl;

    // ---------- if 初始化语句 ----------
    cout << "\n=== if 初始化语句 ===" << endl;
    map<string,int> m = {{"key",42}};
    if (auto it = m.find("key"); it != m.end()) {
        cout << "找到 key=" << it->second << endl;
    }
    if (auto it = m.find("missing"); it == m.end()) {
        cout << "未找到 missing" << endl;
    }

    // switch 初始化语句
    switch (int v = 2; v) {
        case 1: cout << "one\n"; break;
        case 2: cout << "two\n"; break;
        default: cout << "other\n";
    }

    // ---------- std::optional ----------
    cout << "\n=== std::optional ===" << endl;
    auto r1 = safeDiv(10, 3);
    auto r2 = safeDiv(10, 0);

    if (r1) cout << "10/3 = " << *r1 << endl;
    cout << "10/0 有值? " << boolalpha << r2.has_value() << endl;
    cout << "10/0 或默认: " << r2.value_or(-1) << endl;

    map<int,string> users = {{1,"Alice"},{2,"Bob"}};
    cout << findUser(users, 1).value_or("未知") << endl;
    cout << findUser(users, 99).value_or("未知") << endl;

    // ---------- std::variant ----------
    cout << "\n=== std::variant ===" << endl;
    vector<Number> numbers = {42, 3.14, string("hello")};
    for (const auto& n : numbers) printNumber(n);

    // holds_alternative
    Number v = 42;
    cout << "holds int? " << holds_alternative<int>(v) << endl;
    v = string("world");
    cout << "holds string? " << holds_alternative<string>(v) << endl;

    // get_if（安全获取，失败返回 nullptr）
    if (auto* sp = get_if<string>(&v)) {
        cout << "string 值: " << *sp << endl;
    }

    // ---------- std::any ----------
    cout << "\n=== std::any ===" << endl;
    any a = 42;
    cout << "any int: " << any_cast<int>(a) << endl;
    a = string("C++17");
    cout << "any string: " << any_cast<string>(a) << endl;
    a = 3.14;
    cout << "any double: " << any_cast<double>(a) << endl;
    cout << "has_value: " << a.has_value() << endl;

    // ---------- constexpr if ----------
    cout << "\n=== constexpr if ===" << endl;
    describe(42);
    describe(3.14f);
    describe(string("hello"));

    // ---------- std::string_view ----------
    cout << "\n=== string_view ===" << endl;
    string_view sv = "Hello, string_view!";
    cout << "sv: " << sv << endl;
    cout << "子串: " << sv.substr(7, 11) << endl;
    cout << "find: " << sv.find("view") << endl;

    // ---------- 并行算法（C++17） ----------
    cout << "\n=== 并行算法（顺序排序示例） ===" << endl;
    vector<int> big(100000);
    iota(big.begin(), big.end(), 1);

    auto t0 = chrono::high_resolution_clock::now();
    sort(big.begin(), big.end(), greater<int>());
    auto t1 = chrono::high_resolution_clock::now();
    double ms = chrono::duration<double, milli>(t1 - t0).count();
    cout << "降序排序 100K 元素耗时: " << ms << " ms" << endl;
    cout << "前5: ";
    for (int i = 0; i < 5; i++) cout << big[i] << " ";
    cout << endl;

    return 0;
}
