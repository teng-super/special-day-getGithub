/**
 * 【C++ 高级 C++11 - 第4课：移动语义 & 右值引用】
 *
 * 这是 C++11 最重要的特性之一，对性能有巨大影响！
 *
 * 知识点：
 * 1. 左值（lvalue）vs 右值（rvalue）
 * 2. 右值引用（T&&）
 * 3. 移动构造函数（Move Constructor）
 * 4. 移动赋值运算符（Move Assignment Operator）
 * 5. std::move：将左值转为右值引用
 * 6. 完美转发（Perfect Forwarding）：std::forward & 万能引用（T&&）
 * 7. 五法则（Rule of Five）：C++11 中对三法则的扩展
 * 8. 返回值优化（RVO / NRVO）
 * 9. 移动语义对 STL 容器的影响
 */

#include <iostream>
#include <vector>
#include <string>
#include <utility>   // move, forward
#include <chrono>    // 计时

using namespace std;

// ══════════════════════════════════════════════════════════════════
// 演示类：带有详细日志的大型资源类
// ══════════════════════════════════════════════════════════════════
class Buffer {
private:
    char*  data;
    size_t size;
    string name;

    static int instance_count;

public:
    // ── 构造函数 ──────────────────────────────────────────────────
    Buffer(const string& n, size_t sz)
        : data(new char[sz]), size(sz), name(n) {
        fill(data, data + sz, 'X');
        instance_count++;
        cout << "  [构造]    " << name << " (size=" << sz << ")" << endl;
    }

    // ── 析构函数 ──────────────────────────────────────────────────
    ~Buffer() {
        delete[] data;
        instance_count--;
        cout << "  [析构]    " << name << endl;
    }

    // ── 拷贝构造（深拷贝，代价高）────────────────────────────────
    Buffer(const Buffer& other)
        : data(new char[other.size]), size(other.size),
          name(other.name + "_copy") {
        copy(other.data, other.data + other.size, data);
        instance_count++;
        cout << "  [拷贝构造] " << name << " <- " << other.name
             << " (深拷贝 " << size << " 字节)" << endl;
    }

    // ── 拷贝赋值 ──────────────────────────────────────────────────
    Buffer& operator=(const Buffer& other) {
        if (this != &other) {
            delete[] data;
            data = new char[other.size];
            size = other.size;
            copy(other.data, other.data + other.size, data);
            name = other.name + "_assigned";
            cout << "  [拷贝赋值] " << name
                 << " (深拷贝 " << size << " 字节)" << endl;
        }
        return *this;
    }

    // ── 移动构造（"窃取"资源，O(1) 开销）────────────────────────
    Buffer(Buffer&& other) noexcept
        : data(other.data), size(other.size),
          name(move(other.name) + "_moved") {
        other.data = nullptr;  // 将源对象置为无效状态
        other.size = 0;
        instance_count++;
        cout << "  [移动构造] " << name << " (零拷贝！)" << endl;
    }

    // ── 移动赋值 ──────────────────────────────────────────────────
    Buffer& operator=(Buffer&& other) noexcept {
        if (this != &other) {
            delete[] data;     // 释放当前资源
            data = other.data; // 窃取资源
            size = other.size;
            name = move(other.name) + "_move_assigned";
            other.data = nullptr;
            other.size = 0;
            cout << "  [移动赋值] " << name << " (零拷贝！)" << endl;
        }
        return *this;
    }

    // ── 访问 ──────────────────────────────────────────────────────
    size_t     getSize()  const { return size; }
    const char* getData() const { return data; }
    string     getName()  const { return name; }
    bool       valid()    const { return data != nullptr; }

    static int getCount() { return instance_count; }
};

int Buffer::instance_count = 0;

// ══════════════════════════════════════════════════════════════════
// 完美转发（Perfect Forwarding）
// ══════════════════════════════════════════════════════════════════
// 万能引用（Forwarding Reference）：T&& 在模板中不是右值引用！
// 它既能绑定左值也能绑定右值，搭配 forward 保持原始值类别

template <typename T>
void process_value(T&& val) {
    // forward<T>(val)：左值传左值，右值传右值
    cout << "  processing: " << forward<T>(val) << endl;
}

// 完美转发工厂函数（代替 new，类似 make_unique 内部实现）
template <typename T, typename... Args>
T create_object(Args&&... args) {
    return T(forward<Args>(args)...);
}

// ══════════════════════════════════════════════════════════════════
// 返回值优化（RVO）演示
// ══════════════════════════════════════════════════════════════════
Buffer make_buffer(const string& name) {
    Buffer b(name, 1024);
    // 编译器通常直接在调用方栈帧构造（NRVO），不触发移动
    return b;
}

// ══════════════════════════════════════════════════════════════════
// 性能对比：拷贝 vs 移动
// ══════════════════════════════════════════════════════════════════
vector<string> make_large_vector(size_t n) {
    vector<string> v;
    for (size_t i = 0; i < n; i++)
        v.push_back(string(1000, 'A'));  // 每个字符串 1000 字节
    return v;
}

int main() {
    // ─── 1. 左值 vs 右值 ─────────────────────────────────────────
    cout << "=== 左值 vs 右值 ===" << endl;
    // a 是左值（有名字，有地址）；字面量 20 是右值
    // int& lref = 20;  // ❌ 左值引用不能绑定右值（字面量）
    int&& rref = 20;  // ✅ 右值引用绑定右值
    cout << "右值引用 rref = " << rref << endl;

    // ─── 2. Buffer 拷贝 vs 移动 ──────────────────────────────────
    cout << "\n=== 拷贝构造 vs 移动构造 ===" << endl;
    {
        Buffer b1("buf1", 512);

        cout << "\n-- 拷贝构造 --" << endl;
        Buffer b2(b1);          // 调用拷贝构造
        cout << "b1.valid()=" << b1.valid() << " b2.valid()=" << b2.valid() << endl;

        cout << "\n-- 移动构造 --" << endl;
        Buffer b3(move(b1));    // std::move 将 b1 转为右值，触发移动构造
        cout << "b1.valid()=" << b1.valid() << " (b1 被清空)" << endl;
        cout << "b3.valid()=" << b3.valid() << " b3.name=" << b3.getName() << endl;

        cout << "\n-- 移动赋值 --" << endl;
        Buffer b4("buf4", 128);
        b4 = move(b2);          // 移动赋值
        cout << "b2.valid()=" << b2.valid() << endl;
        cout << "b4.name=" << b4.getName() << endl;
    }
    cout << "Buffer 实例数（应为0）：" << Buffer::getCount() << endl;

    // ─── 3. std::move 的本质 ──────────────────────────────────────
    cout << "\n=== std::move 本质 ===" << endl;
    string s1 = "Hello, World!";
    cout << "move 前：s1=\"" << s1 << "\"" << endl;
    string s2 = move(s1);  // s1 被"移走"
    cout << "move 后：s1=\"" << s1 << "\" (已清空)" << endl;
    cout << "         s2=\"" << s2 << "\"" << endl;

    // ─── 4. 完美转发 ─────────────────────────────────────────────
    cout << "\n=== 完美转发 ===" << endl;
    string lval = "左值字符串";
    process_value(lval);           // 传左值
    process_value("右值字符串");   // 传右值（字面量）
    process_value(42);             // 传右值（整数）

    // create_object 完美转发参数
    auto buf = create_object<Buffer>("forwarded", 256);

    // ─── 5. RVO 演示 ─────────────────────────────────────────────
    cout << "\n=== 返回值优化（RVO）===" << endl;
    {
        Buffer b = make_buffer("rvo_buf");  // 可能不触发任何拷贝/移动
        cout << "得到 Buffer：" << b.getName() << " size=" << b.getSize() << endl;
    }

    // ─── 6. vector 中的移动语义 ──────────────────────────────────
    cout << "\n=== vector 中的移动语义 ===" << endl;
    {
        vector<Buffer> buffers;
        buffers.reserve(3);  // 避免扩容时触发不必要的移动

        cout << "emplace_back（直接在 vector 内构造）：" << endl;
        buffers.emplace_back("v_buf1", 64);   // 直接构造，无拷贝/移动
        buffers.emplace_back("v_buf2", 128);
        buffers.emplace_back("v_buf3", 256);

        cout << "\npush_back with move：" << endl;
        Buffer temp("temp", 512);
        buffers.push_back(move(temp));  // 移动进 vector
        cout << "temp.valid()=" << temp.valid() << endl;
    }
    cout << "vector 析构，Buffer 实例数：" << Buffer::getCount() << endl;

    // ─── 7. 性能对比：拷贝 vs 移动大向量 ────────────────────────
    cout << "\n=== 性能对比：string vector 拷贝 vs 移动 ===" << endl;
    const size_t N = 10000;

    vector<string> original = make_large_vector(N);

    // 拷贝
    auto t1 = chrono::high_resolution_clock::now();
    vector<string> copied = original;  // 深拷贝
    auto t2 = chrono::high_resolution_clock::now();

    // 移动
    auto t3 = chrono::high_resolution_clock::now();
    vector<string> moved = move(original);  // O(1) 移动
    auto t4 = chrono::high_resolution_clock::now();

    auto us_copy = chrono::duration_cast<chrono::microseconds>(t2 - t1).count();
    auto us_move = chrono::duration_cast<chrono::microseconds>(t4 - t3).count();

    cout << "拷贝 " << N << " 个字符串耗时：" << us_copy << " μs" << endl;
    cout << "移动 " << N << " 个字符串耗时：" << us_move << " μs" << endl;
    cout << "加速比：" << (us_move > 0 ? us_copy / us_move : static_cast<long long>(N)) << "×" << endl;
    cout << "original.size()=" << original.size() << " (被移走后为0)" << endl;
    cout << "moved.size()=" << moved.size() << endl;

    return 0;
}
