/**
 * 【C++ 高级 C++11 - 第3课：智能指针 & 内存管理】
 *
 * 知识点：
 * 1. 为什么需要智能指针：RAII 与内存泄漏
 * 2. unique_ptr：独占所有权
 *    - make_unique（C++14）
 *    - 移动语义传递所有权
 *    - 自定义删除器
 * 3. shared_ptr：共享所有权（引用计数）
 *    - make_shared（优先使用）
 *    - 引用计数的机制
 * 4. weak_ptr：打破循环引用
 *    - 不增加引用计数
 *    - lock() 获取 shared_ptr
 * 5. 循环引用问题与解决方案
 * 6. enable_shared_from_this
 * 7. 智能指针最佳实践
 */

#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include <functional>

using namespace std;

// ══════════════════════════════════════════════════════════════════
// 测试类（打印构造/析构日志）
// ══════════════════════════════════════════════════════════════════
class Resource {
private:
    string name;
    int    value;

public:
    Resource(const string& n, int v) : name(n), value(v) {
        cout << "  [构造] Resource(\"" << name << "\", " << value << ")" << endl;
    }

    ~Resource() {
        cout << "  [析构] ~Resource(\"" << name << "\")" << endl;
    }

    void use() const {
        cout << "  使用 Resource: " << name << "=" << value << endl;
    }

    string getName()  const { return name; }
    int    getValue() const { return value; }
};

// ══════════════════════════════════════════════════════════════════
// 循环引用演示
// ══════════════════════════════════════════════════════════════════
struct Node {
    string              name;
    shared_ptr<Node>    next;  // ❌ 会造成循环引用
    weak_ptr<Node>      prev;  // ✅ weak_ptr 不增加引用计数

    Node(const string& n) : name(n) {
        cout << "  [构造] Node(\"" << name << "\")" << endl;
    }
    ~Node() {
        cout << "  [析构] ~Node(\"" << name << "\")" << endl;
    }
};

// ══════════════════════════════════════════════════════════════════
// enable_shared_from_this：在类内部获取 shared_ptr<this>
// ══════════════════════════════════════════════════════════════════
class Widget : public enable_shared_from_this<Widget> {
private:
    string id;

public:
    Widget(const string& i) : id(i) {
        cout << "  [构造] Widget(\"" << id << "\")" << endl;
    }
    ~Widget() {
        cout << "  [析构] ~Widget(\"" << id << "\")" << endl;
    }

    // 在成员函数中安全地返回 shared_ptr<this>
    shared_ptr<Widget> getSharedPtr() {
        return shared_from_this();
    }

    void show() const { cout << "  Widget: " << id << endl; }
};

// ══════════════════════════════════════════════════════════════════
// main
// ══════════════════════════════════════════════════════════════════
int main() {
    // ─── 1. unique_ptr：独占所有权 ───────────────────────────────
    cout << "=== unique_ptr ===" << endl;
    {
        cout << "-- 创建 unique_ptr --" << endl;
        // C++14 make_unique（推荐）
        auto p1 = make_unique<Resource>("res1", 100);
        p1->use();

        // C++11 方式（不推荐，有异常安全问题）
        unique_ptr<Resource> p2(new Resource("res2", 200));
        p2->use();

        cout << "\n-- unique_ptr 不能拷贝 --" << endl;
        // auto p3 = p1;  // ❌ 编译错误：unique_ptr 不可拷贝

        cout << "\n-- 但可以移动（所有权转移）--" << endl;
        auto p3 = move(p1);  // p1 变为 nullptr，p3 接管
        if (!p1) cout << "  p1 变为 nullptr（所有权已转移）" << endl;
        p3->use();

        cout << "\n-- 作用域结束，自动释放 --" << endl;
    }

    // ─── 2. unique_ptr 数组 ───────────────────────────────────────
    cout << "\n=== unique_ptr 数组 ===" << endl;
    {
        auto arr = make_unique<int[]>(5);
        for (int i = 0; i < 5; i++) arr[i] = (i + 1) * 10;
        cout << "数组：";
        for (int i = 0; i < 5; i++) cout << arr[i] << " ";
        cout << endl;
    }

    // ─── 3. unique_ptr 自定义删除器 ──────────────────────────────
    cout << "\n=== 自定义删除器 ===" << endl;
    {
        auto custom_deleter = [](Resource* p) {
            cout << "  [自定义删除器] 清理资源 \"" << p->getName() << "\"" << endl;
            delete p;
        };

        unique_ptr<Resource, decltype(custom_deleter)>
            p(new Resource("custom_res", 999), custom_deleter);
        p->use();
    }

    // ─── 4. shared_ptr：共享所有权 ───────────────────────────────
    cout << "\n=== shared_ptr ===" << endl;
    {
        cout << "-- 创建 shared_ptr --" << endl;
        auto sp1 = make_shared<Resource>("shared_res", 42);
        cout << "  引用计数：" << sp1.use_count() << endl;  // 1

        {
            auto sp2 = sp1;  // 共享所有权，引用计数+1
            auto sp3 = sp1;
            cout << "  创建 sp2, sp3 后，引用计数：" << sp1.use_count() << endl; // 3
            sp1->use();

            // 通过任何一个指针都能访问
            sp2->use();
        }
        // sp2, sp3 析构，引用计数减回1
        cout << "  sp2, sp3 析构后，引用计数：" << sp1.use_count() << endl; // 1
    }
    // sp1 析构，引用计数=0，对象被销毁

    // ─── 5. weak_ptr：打破循环引用 ───────────────────────────────
    cout << "\n=== 循环引用问题 ===" << endl;
    {
        cout << "-- 演示循环引用（内存泄漏）--" << endl;
        {
            auto a = make_shared<Node>("NodeA");
            auto b = make_shared<Node>("NodeB");
            // ❌ 循环引用：a->next = b 且 b->next = a
            // 导致 a 和 b 的引用计数永远不为0，无法析构
            // a->next = b;
            // b->next = a;
            cout << "  （循环引用代码已注释，避免内存泄漏演示）" << endl;
            cout << "  a 引用计数：" << a.use_count() << endl;
            cout << "  b 引用计数：" << b.use_count() << endl;
        }
        cout << "  -- 使用 weak_ptr 修复循环引用 --" << endl;
        {
            auto a = make_shared<Node>("NodeA");
            auto b = make_shared<Node>("NodeB");
            a->next = b;        // shared_ptr：b 引用计数+1
            b->prev = a;        // weak_ptr：a 引用计数不变！

            cout << "  a 引用计数：" << a.use_count() << endl;  // 1
            cout << "  b 引用计数：" << b.use_count() << endl;  // 2 (a->next)

            // weak_ptr 使用前需要 lock()
            if (auto locked = b->prev.lock()) {
                cout << "  通过 weak_ptr 访问：" << locked->name << endl;
            }
        }
        cout << "  -- 作用域结束，正确析构 --" << endl;
    }

    // ─── 6. enable_shared_from_this ──────────────────────────────
    cout << "\n=== enable_shared_from_this ===" << endl;
    {
        auto w1 = make_shared<Widget>("W1");
        w1->show();

        auto w2 = w1->getSharedPtr();  // 从内部获取 shared_ptr
        cout << "  w1 引用计数：" << w1.use_count() << endl;  // 2

        w2->show();
    }

    // ─── 7. shared_ptr 用于多态 ──────────────────────────────────
    cout << "\n=== shared_ptr 多态 ===" << endl;
    {
        // 基类指针存储派生类对象（用 unique_ptr 更常见）
        vector<shared_ptr<Resource>> pool;
        for (int i = 1; i <= 3; i++) {
            pool.push_back(make_shared<Resource>("poolRes" + to_string(i), i * 10));
        }
        cout << "资源池大小：" << pool.size() << endl;
        for (auto& r : pool) {
            r->use();
            cout << "    引用计数：" << r.use_count() << endl;
        }
    }
    cout << "资源池析构，所有资源释放" << endl;

    // ─── 8. 智能指针最佳实践总结 ─────────────────────────────────
    cout << "\n=== 最佳实践总结 ===" << endl;
    cout << "1. 优先用 unique_ptr，不需要共享时不用 shared_ptr" << endl;
    cout << "2. 用 make_unique / make_shared 代替 new" << endl;
    cout << "3. 出现循环引用时，用 weak_ptr 打破" << endl;
    cout << "4. 函数参数：传值/引用，而非 shared_ptr（避免不必要的引用计数）" << endl;
    cout << "5. 不要将裸指针和智能指针混用" << endl;

    return 0;
}
