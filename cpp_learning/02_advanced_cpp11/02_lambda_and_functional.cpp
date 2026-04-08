/**
 * 【C++ 高级 C++11 - 第2课：Lambda 表达式 & 函数式编程】
 *
 * 知识点：
 * 1. Lambda 表达式语法：[capture](params) -> return_type { body }
 * 2. 捕获列表：
 *    - []     不捕获任何外部变量
 *    - [=]    值捕获所有外部变量（拷贝）
 *    - [&]    引用捕获所有外部变量
 *    - [x, &y] 值捕获 x，引用捕获 y
 *    - [this] 捕获当前对象指针
 * 3. mutable lambda：修改值捕获的变量
 * 4. 泛型 lambda（C++14）：auto 参数
 * 5. std::function：类型擦除的可调用包装器
 * 6. std::bind：绑定函数参数
 * 7. 高阶函数：map / filter / reduce 模拟
 * 8. 闭包（Closure）
 */

#include <iostream>
#include <vector>
#include <functional>  // function, bind
#include <algorithm>
#include <numeric>
#include <string>
#include <map>

using namespace std;

// ══════════════════════════════════════════════════════════════════
// 辅助：打印 vector
// ══════════════════════════════════════════════════════════════════
template <typename T>
void print_vec(const vector<T>& v, const string& label = "") {
    if (!label.empty()) cout << label << ": ";
    cout << "[";
    for (size_t i = 0; i < v.size(); i++) {
        if (i) cout << ", ";
        cout << v[i];
    }
    cout << "]" << endl;
}

// ══════════════════════════════════════════════════════════════════
// 高阶函数工具（函数式编程风格）
// ══════════════════════════════════════════════════════════════════

// map：对每个元素应用 f，返回新 vector
template <typename T, typename Func>
vector<T> map_fn(const vector<T>& v, Func f) {
    vector<T> result;
    result.reserve(v.size());
    for (const T& x : v) result.push_back(f(x));
    return result;
}

// filter：保留满足 pred 的元素
template <typename T, typename Pred>
vector<T> filter_fn(const vector<T>& v, Pred pred) {
    vector<T> result;
    for (const T& x : v)
        if (pred(x)) result.push_back(x);
    return result;
}

// reduce（fold left）：累计操作
template <typename T, typename Func>
T reduce_fn(const vector<T>& v, T init, Func f) {
    T acc = init;
    for (const T& x : v) acc = f(acc, x);
    return acc;
}

// ══════════════════════════════════════════════════════════════════
// 闭包工厂：返回 lambda 的函数
// ══════════════════════════════════════════════════════════════════
// 生成"加 n"的函数
function<int(int)> make_adder(int n) {
    return [n](int x) { return x + n; };  // 捕获 n
}

// 生成"乘 n"的函数
function<int(int)> make_multiplier(int n) {
    return [n](int x) { return x * n; };
}

// 计数器：每次调用递增
function<int()> make_counter(int start = 0) {
    int count = start;
    return [count]() mutable { return count++; };  // mutable 修改捕获值
}

// 函数组合：compose(f, g)(x) = f(g(x))
function<int(int)> compose(function<int(int)> f, function<int(int)> g) {
    return [f, g](int x) { return f(g(x)); };
}

// ══════════════════════════════════════════════════════════════════
// 类中使用 lambda
// ══════════════════════════════════════════════════════════════════
class EventSystem {
private:
    map<string, vector<function<void(const string&)>>> listeners;

public:
    // 注册事件监听器
    void on(const string& event, function<void(const string&)> handler) {
        listeners[event].push_back(handler);
    }

    // 触发事件
    void emit(const string& event, const string& data = "") {
        auto it = listeners.find(event);
        if (it != listeners.end()) {
            for (auto& h : it->second) h(data);
        }
    }
};

int main() {
    // ─── 1. 基本 Lambda ───────────────────────────────────────────
    cout << "=== 基本 Lambda ===" << endl;

    // 最简单的 lambda
    auto greet = []() { cout << "Hello from lambda!" << endl; };
    greet();

    // 带参数和返回值
    auto add = [](int a, int b) -> int { return a + b; };
    cout << "add(3, 4) = " << add(3, 4) << endl;

    // 省略返回类型（自动推断）
    auto square = [](double x) { return x * x; };
    cout << "square(5.0) = " << square(5.0) << endl;

    // ─── 2. 捕获列表 ──────────────────────────────────────────────
    cout << "\n=== 捕获列表 ===" << endl;
    int  base  = 10;
    string msg = "captured";

    // 值捕获
    auto f1 = [base, msg]() {
        cout << "值捕获：base=" << base << " msg=" << msg << endl;
    };
    base = 99;  // 修改外部，不影响 f1 内的副本
    f1();
    cout << "外部 base 已变为 " << base << endl;

    // 引用捕获
    int counter = 0;
    auto f2 = [&counter]() { counter++; };
    f2(); f2(); f2();
    cout << "引用捕获，counter=" << counter << " (调用3次)" << endl;

    // 全部值捕获 [=]
    int x = 1, y = 2, z = 3;
    auto f3 = [=]() { return x + y + z; };
    cout << "全部值捕获 [=]：x+y+z=" << f3() << endl;

    // ─── 3. mutable lambda ────────────────────────────────────────
    cout << "\n=== mutable lambda ===" << endl;
    int val = 0;
    auto increment = [val]() mutable {
        val++;           // 修改的是 lambda 内部的副本
        return val;
    };
    cout << "increment(): " << increment() << endl;  // 1
    cout << "increment(): " << increment() << endl;  // 2
    cout << "外部 val 未变：" << val << endl;         // 0

    // ─── 4. Lambda 排序 ──────────────────────────────────────────
    cout << "\n=== Lambda 排序 ===" << endl;
    vector<pair<string, int>> people = {
        {"Alice", 30}, {"Bob", 25}, {"Charlie", 35}, {"Dave", 28}
    };

    // 按年龄升序
    sort(people.begin(), people.end(),
         [](const auto& a, const auto& b) { return a.second < b.second; });

    cout << "按年龄升序：" << endl;
    for (const auto& [name, age] : people)
        cout << "  " << name << ": " << age << endl;

    // 按名字长度排序
    sort(people.begin(), people.end(),
         [](const auto& a, const auto& b) {
             return a.first.length() < b.first.length();
         });
    cout << "按名字长度：" << endl;
    for (const auto& [name, age] : people)
        cout << "  " << name << " (len=" << name.length() << ")" << endl;

    // ─── 5. 函数式编程：map/filter/reduce ────────────────────────
    cout << "\n=== 函数式风格 ===" << endl;
    vector<int> nums = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    print_vec(nums, "原始");

    auto doubled = map_fn(nums, [](int x) { return x * 2; });
    print_vec(doubled, "×2");

    auto evens = filter_fn(nums, [](int x) { return x % 2 == 0; });
    print_vec(evens, "偶数");

    auto sum = reduce_fn(nums, 0, [](int acc, int x) { return acc + x; });
    cout << "求和（reduce）: " << sum << endl;

    // 链式：先过滤偶数，再平方，再求和
    auto even_nums = filter_fn(nums, [](int x){ return x % 2 == 0; });
    auto squared   = map_fn(even_nums, [](int x){ return x * x; });
    auto total     = reduce_fn(squared, 0, [](int a, int b){ return a + b; });
    cout << "偶数平方和：" << total << endl;

    // ─── 6. 闭包工厂 ─────────────────────────────────────────────
    cout << "\n=== 闭包工厂 ===" << endl;
    auto add5  = make_adder(5);
    auto add10 = make_adder(10);
    auto mul3  = make_multiplier(3);

    cout << "add5(7)  = " << add5(7)  << endl;
    cout << "add10(7) = " << add10(7) << endl;
    cout << "mul3(7)  = " << mul3(7)  << endl;

    // 函数组合：先乘3再加5 = compose(add5, mul3)
    auto mul3_then_add5 = compose(add5, mul3);
    cout << "mul3_then_add5(4) = " << mul3_then_add5(4) << " (4*3+5=17)" << endl;

    // ─── 7. make_counter（闭包保持状态）─────────────────────────
    cout << "\n=== 闭包计数器 ===" << endl;
    auto cnt1 = make_counter(0);
    auto cnt2 = make_counter(100);
    for (int i = 0; i < 5; i++) cout << cnt1() << " ";
    cout << endl;
    for (int i = 0; i < 3; i++) cout << cnt2() << " ";
    cout << endl;

    // ─── 8. std::function & std::bind ─────────────────────────────
    cout << "\n=== std::function & std::bind ===" << endl;
    function<int(int, int)> fn;

    fn = [](int a, int b) { return a + b; };
    cout << "fn = lambda add: fn(3,4) = " << fn(3, 4) << endl;

    // bind：将参数绑定（创建偏函数）
    auto add_to_10 = bind(fn, 10, placeholders::_1);  // 固定第一个参数为10
    cout << "add_to_10(5) = " << add_to_10(5)  << endl;
    cout << "add_to_10(7) = " << add_to_10(7)  << endl;

    // ─── 9. 事件系统（Lambda 作为回调）──────────────────────────
    cout << "\n=== 事件系统（Lambda 回调）===" << endl;
    EventSystem events;
    int click_count = 0;

    events.on("click", [&click_count](const string& data) {
        click_count++;
        cout << "点击事件（第" << click_count << "次）：" << data << endl;
    });

    events.on("click", [](const string& data) {
        cout << "另一个点击监听器：" << data << endl;
    });

    events.on("hover", [](const string& data) {
        cout << "悬停事件：" << data << endl;
    });

    events.emit("click", "按钮A");
    events.emit("hover", "菜单项");
    events.emit("click", "按钮B");
    cout << "总点击次数：" << click_count << endl;

    // ─── 10. 泛型 lambda（C++14）────────────────────────────────
    cout << "\n=== 泛型 lambda（C++14）===" << endl;
    auto generic_print = [](const auto& x) { cout << x << endl; };
    generic_print(42);
    generic_print(3.14);
    generic_print(string("C++14"));

    auto generic_add = [](auto a, auto b) { return a + b; };
    cout << "generic_add(1, 2) = "       << generic_add(1, 2)       << endl;
    cout << "generic_add(1.5, 2.5) = "   << generic_add(1.5, 2.5)   << endl;
    cout << "generic_add(\"Hi \",\"C++\") = "
         << generic_add(string("Hi "), string("C++")) << endl;

    return 0;
}
