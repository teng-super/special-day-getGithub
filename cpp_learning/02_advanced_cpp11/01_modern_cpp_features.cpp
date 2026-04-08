/**
 * 【C++ 高级 C++11 - 第1课：现代 C++ 核心特性】
 *
 * 知识点：
 * 1. auto 类型推断
 * 2. 范围 for 循环（Range-based for）
 * 3. nullptr（替代 NULL）
 * 4. 统一初始化（Uniform Initialization）{}
 * 5. initializer_list
 * 6. constexpr（编译期常量表达式）
 * 7. static_assert（编译期断言）
 * 8. decltype（类型推断）
 * 9. 强类型枚举（enum class）
 * 10. 委托构造函数（Delegating Constructor）
 * 11. 继承构造函数（Inheriting Constructor）
 * 12. override / final
 */

#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <initializer_list>
#include <type_traits>  // is_integral, is_floating_point
#include <cstdint>      // uint8_t
#include <cmath>        // sqrt

using namespace std;

// ══════════════════════════════════════════════════════════════════
// 1. constexpr：编译期求值
// ══════════════════════════════════════════════════════════════════
constexpr int factorial(int n) {
    return n <= 1 ? 1 : n * factorial(n - 1);
}

constexpr double PI = 3.14159265358979;

constexpr double circle_area(double r) {
    return PI * r * r;
}

// ══════════════════════════════════════════════════════════════════
// 2. initializer_list：支持 {} 初始化多个值
// ══════════════════════════════════════════════════════════════════
class NumberList {
private:
    vector<int> data;

public:
    // 接受初始化列表
    NumberList(initializer_list<int> il) : data(il) {
        cout << "NumberList 初始化，共 " << il.size() << " 个元素" << endl;
    }

    double average() const {
        if (data.empty()) return 0;
        double sum = 0;
        for (int x : data) sum += x;
        return sum / data.size();
    }

    void print() const {
        cout << "[";
        for (size_t i = 0; i < data.size(); i++) {
            if (i) cout << ", ";
            cout << data[i];
        }
        cout << "]" << endl;
    }
};

// ══════════════════════════════════════════════════════════════════
// 3. enum class（强类型枚举）
// ══════════════════════════════════════════════════════════════════
// 传统枚举（容易命名污染）
// enum Direction { UP, DOWN, LEFT, RIGHT };  // UP 是全局符号

// 强类型枚举：必须加作用域 Direction::UP
enum class Direction : uint8_t {
    UP    = 0,
    DOWN  = 1,
    LEFT  = 2,
    RIGHT = 3
};

enum class Color : int {
    RED   = 0xFF0000,
    GREEN = 0x00FF00,
    BLUE  = 0x0000FF
};

string direction_name(Direction d) {
    switch (d) {
        case Direction::UP:    return "上";
        case Direction::DOWN:  return "下";
        case Direction::LEFT:  return "左";
        case Direction::RIGHT: return "右";
        default:               return "未知";
    }
}

// ══════════════════════════════════════════════════════════════════
// 4. 委托构造函数（Delegating Constructor）
// ══════════════════════════════════════════════════════════════════
class Config {
private:
    string host;
    int    port;
    string dbname;
    int    timeout;

public:
    // 主构造函数
    Config(const string& h, int p, const string& db, int t)
        : host(h), port(p), dbname(db), timeout(t) {}

    // 委托给主构造函数（避免重复代码）
    Config(const string& h, int p, const string& db)
        : Config(h, p, db, 30) {}            // 默认超时 30s

    Config(const string& h, int p)
        : Config(h, p, "default") {}         // 默认数据库名

    Config()
        : Config("localhost", 5432) {}       // 完全默认

    void print() const {
        cout << "Config{host=" << host
             << " port=" << port
             << " db=" << dbname
             << " timeout=" << timeout << "s}" << endl;
    }
};

// ══════════════════════════════════════════════════════════════════
// 5. decltype：从表达式推断类型
// ══════════════════════════════════════════════════════════════════
template <typename T, typename U>
auto multiply(T a, U b) -> decltype(a * b) {
    return a * b;
}

// ══════════════════════════════════════════════════════════════════
// 6. static_assert：编译期断言
// ══════════════════════════════════════════════════════════════════
static_assert(sizeof(int) >= 4, "int 必须至少 4 字节！");
static_assert(factorial(5) == 120, "factorial(5) 必须等于 120");

// 模板约束（C++11 风格）
template <typename T>
T safe_sqrt(T val) {
    static_assert(is_floating_point<T>::value,
                  "safe_sqrt 只接受浮点类型");
    return val >= 0 ? sqrt(val) : 0;
}

// ══════════════════════════════════════════════════════════════════
// main
// ══════════════════════════════════════════════════════════════════
int main() {
    // ─── auto ─────────────────────────────────────────────────────
    cout << "=== auto 类型推断 ===" << endl;
    auto i = 42;            // int
    auto d = 3.14;          // double
    auto s = string("Hi");  // string
    auto v = vector<int>{1, 2, 3};  // vector<int>

    cout << "auto i=" << i << " d=" << d << " s=" << s << endl;

    // auto 遍历 map
    map<string, int> m = {{"a", 1}, {"b", 2}, {"c", 3}};
    for (const auto& [key, val] : m) {  // C++17 结构化绑定
        cout << key << "=" << val << " ";
    }
    cout << endl;

    // ─── 范围 for ─────────────────────────────────────────────────
    cout << "\n=== 范围 for 循环 ===" << endl;
    vector<int> nums = {10, 20, 30, 40, 50};
    for (int x : nums) cout << x << " ";
    cout << endl;

    // 修改元素：必须用引用
    for (int& x : nums) x *= 2;
    for (int x : nums) cout << x << " ";
    cout << endl;

    // ─── nullptr ──────────────────────────────────────────────────
    cout << "\n=== nullptr ===" << endl;
    int* ptr = nullptr;
    cout << "ptr == nullptr ? " << (ptr == nullptr ? "是" : "否") << endl;
    // nullptr_t 类型，不是整数，比 NULL 更安全
    // 避免 void f(int) 和 void f(int*) 调用时 NULL 歧义

    // ─── 统一初始化 {} ────────────────────────────────────────────
    cout << "\n=== 统一初始化 {} ===" << endl;
    int    a{10};
    double b{3.14};
    string str{"Hello"};
    vector<int> vi{1, 2, 3, 4, 5};
    // int c{3.14};  // ❌ 窄化转换，编译错误！（比 () 更安全）

    cout << "a=" << a << " b=" << b << " str=" << str << endl;
    cout << "vi: ";
    for (int x : vi) cout << x << " ";
    cout << endl;

    // ─── initializer_list ─────────────────────────────────────────
    cout << "\n=== initializer_list ===" << endl;
    NumberList nl = {3, 1, 4, 1, 5, 9, 2, 6};
    nl.print();
    cout << "平均值：" << nl.average() << endl;

    // ─── constexpr ────────────────────────────────────────────────
    cout << "\n=== constexpr（编译期计算）===" << endl;
    // 编译期计算，运行时常量
    constexpr int f5 = factorial(5);
    constexpr int f10 = factorial(10);
    cout << "5! = " << f5 << endl;
    cout << "10! = " << f10 << endl;
    cout << "圆面积(r=5) = " << circle_area(5) << endl;

    // 数组大小可用 constexpr（不能用运行时变量）
    constexpr int SIZE = 8;
    int arr[SIZE];  // ✅
    for (int i = 0; i < SIZE; i++) arr[i] = i * i;
    for (int i = 0; i < SIZE; i++) cout << arr[i] << " ";
    cout << endl;

    // ─── enum class ───────────────────────────────────────────────
    cout << "\n=== enum class ===" << endl;
    Direction dir = Direction::UP;
    cout << "方向：" << direction_name(dir) << endl;
    dir = Direction::RIGHT;
    cout << "方向：" << direction_name(dir) << endl;

    // 获取底层值
    cout << "Direction::LEFT 的值：" << static_cast<int>(Direction::LEFT) << endl;
    cout << "Color::RED   的值：0x" << hex << static_cast<int>(Color::RED) << dec << endl;

    // ─── 委托构造函数 ─────────────────────────────────────────────
    cout << "\n=== 委托构造函数 ===" << endl;
    Config c1;
    Config c2("db.example.com", 3306);
    Config c3("db.example.com", 3306, "mydb");
    Config c4("db.example.com", 3306, "mydb", 60);
    c1.print();
    c2.print();
    c3.print();
    c4.print();

    // ─── decltype ─────────────────────────────────────────────────
    cout << "\n=== decltype ===" << endl;
    auto r1 = multiply(3, 4);      // int
    auto r2 = multiply(3, 4.5);    // double
    auto r3 = multiply(2.0f, 3);   // float
    cout << "3 * 4     = " << r1 << " (int)"    << endl;
    cout << "3 * 4.5   = " << r2 << " (double)" << endl;
    cout << "2.0f * 3  = " << r3 << " (float)"  << endl;

    // ─── safe_sqrt ────────────────────────────────────────────────
    cout << "\n=== safe_sqrt（static_assert 约束）===" << endl;
    cout << "safe_sqrt(2.0)  = " << safe_sqrt(2.0)  << endl;
    cout << "safe_sqrt(-1.0) = " << safe_sqrt(-1.0) << " (负数返回0)" << endl;
    // safe_sqrt(2);   // ❌ 编译错误：只接受浮点类型

    return 0;
}
