/**
 * 01_cpp11_core.cpp
 * C++11 核心新特性
 * 演示：auto、decltype、nullptr、范围 for、初始化列表、
 *       委托构造、override/final、强类型枚举
 */

#include <iostream>
#include <vector>
#include <map>
#include <initializer_list>
#include <type_traits>
using namespace std;

// ---------- 初始化列表构造 ----------
class NumberList {
    vector<int> data;
public:
    NumberList(initializer_list<int> il) : data(il) {}
    void print() const {
        for (int v : data) cout << v << " ";
        cout << endl;
    }
};

// ---------- 委托构造函数 ----------
class Config {
    string host;
    int    port;
    bool   debug;
public:
    // 主构造
    Config(const string& h, int p, bool d) : host(h), port(p), debug(d) {}
    // 委托给主构造
    Config(const string& h, int p) : Config(h, p, false) {}
    Config() : Config("localhost", 8080) {}
    void print() const {
        cout << "Config{" << host << ":" << port
             << ", debug=" << boolalpha << debug << "}" << endl;
    }
};

// ---------- override / final ----------
class Base {
public:
    virtual void func()       { cout << "Base::func" << endl; }
    virtual void canOverride() {}
    virtual ~Base() = default;
};

class Derived : public Base {
public:
    void func() override      { cout << "Derived::func" << endl; }
    // void func() final 也可以防止再次继承后重写
};

// ---------- 强类型枚举 enum class ----------
enum class Color { Red, Green, Blue };
enum class Direction { North, South, East, West };

// ======================================================
int main() {
    // ---------- auto ----------
    cout << "=== auto ===" << endl;
    auto i    = 42;           // int
    auto d    = 3.14;         // double
    auto s    = string("hi"); // std::string
    auto vec  = vector<int>{1, 2, 3};
    cout << "auto int: " << i << ", double: " << d << ", string: " << s << endl;

    // ---------- decltype ----------
    cout << "\n=== decltype ===" << endl;
    decltype(i) j = 100;    // j 的类型与 i 相同（int）
    cout << "decltype(i) j = " << j << endl;

    // ---------- nullptr ----------
    cout << "\n=== nullptr ===" << endl;
    int* ptr = nullptr;
    // C++11 中 NULL 可能是 0（整数），nullptr 是指针类型，更安全
    cout << "ptr is nullptr: " << boolalpha << (ptr == nullptr) << endl;

    // ---------- 范围 for ----------
    cout << "\n=== 范围 for ===" << endl;
    vector<int> nums = {10, 20, 30, 40, 50};
    for (const auto& v : nums) cout << v << " ";
    cout << endl;

    map<string,int> m = {{"a",1},{"b",2},{"c",3}};
    for (const auto& [key, val] : m) {   // C++17 结构化绑定
        cout << key << "=" << val << " ";
    }
    cout << endl;

    // ---------- 初始化列表 ----------
    cout << "\n=== 初始化列表 ===" << endl;
    NumberList nl = {1, 2, 3, 4, 5};
    nl.print();

    // ---------- 委托构造 ----------
    cout << "\n=== 委托构造 ===" << endl;
    Config c1, c2("192.168.1.1", 9090), c3("srv", 443, true);
    c1.print(); c2.print(); c3.print();

    // ---------- override ----------
    cout << "\n=== override ===" << endl;
    Base* bp = new Derived();
    bp->func();
    delete bp;

    // ---------- 强类型枚举 ----------
    cout << "\n=== enum class ===" << endl;
    Color    col = Color::Blue;
    Direction dir = Direction::North;
    // col == dir  // 编译错误！不同枚举类型不可比较
    if (col == Color::Blue)    cout << "颜色：蓝色" << endl;
    if (dir == Direction::North) cout << "方向：北" << endl;
    cout << "Color::Red 底层值: " << static_cast<int>(Color::Red) << endl;

    return 0;
}
