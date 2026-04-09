/**
 * 04_move_semantics.cpp
 * C++11 移动语义与右值引用
 * 演示：左值/右值、右值引用、移动构造/赋值、std::move、
 *       完美转发（std::forward）、返回值优化（RVO/NRVO）
 */

#include <iostream>
#include <string>
#include <utility>   // std::move, std::forward
#include <vector>
using namespace std;

// ---------- 带资源的类，演示移动语义 ----------
class Buffer {
    char*  data;
    size_t size;
public:
    // 普通构造
    explicit Buffer(size_t n) : data(new char[n]()), size(n) {
        cout << "[Buffer] 构造, size=" << size << endl;
    }

    // 拷贝构造（深拷贝，代价高）
    Buffer(const Buffer& other) : data(new char[other.size]()), size(other.size) {
        copy(other.data, other.data + size, data);
        cout << "[Buffer] 拷贝构造, size=" << size << endl;
    }

    // 移动构造（"窃取"资源，O(1) 代价）
    Buffer(Buffer&& other) noexcept : data(other.data), size(other.size) {
        other.data = nullptr;
        other.size = 0;
        cout << "[Buffer] 移动构造, size=" << size << endl;
    }

    // 拷贝赋值
    Buffer& operator=(const Buffer& other) {
        if (this != &other) {
            delete[] data;
            size = other.size;
            data = new char[size]();
            copy(other.data, other.data + size, data);
            cout << "[Buffer] 拷贝赋值, size=" << size << endl;
        }
        return *this;
    }

    // 移动赋值
    Buffer& operator=(Buffer&& other) noexcept {
        if (this != &other) {
            delete[] data;
            data = other.data;
            size = other.size;
            other.data = nullptr;
            other.size = 0;
            cout << "[Buffer] 移动赋值, size=" << size << endl;
        }
        return *this;
    }

    ~Buffer() {
        if (data) {
            delete[] data;
            cout << "[Buffer] 析构, size=" << size << endl;
        } else {
            cout << "[Buffer] 析构（已移走）" << endl;
        }
    }

    size_t getSize() const { return size; }
};

// ---------- 完美转发示例 ----------
void processValue(int& v)  { cout << "左值引用: " << v << endl; }
void processValue(int&& v) { cout << "右值引用: " << v << endl; }

template <typename T>
void forwardDemo(T&& arg) {
    processValue(forward<T>(arg));  // 保持参数的值类别
}

// ---------- 返回 Buffer（RVO 优化） ----------
Buffer makeBuffer(size_t n) {
    Buffer b(n);
    return b;   // NRVO：编译器直接在调用方的内存中构造，省去拷贝
}

// ======================================================
int main() {
    cout << "=== 拷贝 vs 移动 ===" << endl;
    Buffer b1(100);
    Buffer b2(b1);         // 拷贝构造
    Buffer b3(move(b1));   // 移动构造，b1 之后不可再用
    cout << "b1.size after move: " << b1.getSize() << endl;  // 0
    cout << "b3.size: " << b3.getSize() << endl;             // 100

    cout << "\n=== 移动赋值 ===" << endl;
    Buffer b4(50);
    b4 = move(b3);   // 移动赋值
    cout << "b3.size after move: " << b3.getSize() << endl;  // 0
    cout << "b4.size: " << b4.getSize() << endl;             // 100

    cout << "\n=== std::move 在 vector 中的效果 ===" << endl;
    vector<Buffer> vec;
    vec.reserve(3);
    vec.push_back(Buffer(10));   // 右值，直接移动
    Buffer tmp(20);
    vec.push_back(move(tmp));    // 显式移动
    cout << "vec 大小: " << vec.size() << endl;

    cout << "\n=== 完美转发 ===" << endl;
    int lval = 42;
    forwardDemo(lval);       // 传左值 → 左值引用
    forwardDemo(100);        // 传右值 → 右值引用

    cout << "\n=== RVO（返回值优化） ===" << endl;
    Buffer b5 = makeBuffer(200);  // 编译器直接在 b5 处构造，无需拷贝/移动
    cout << "b5.size: " << b5.getSize() << endl;

    // ---------- std::string 移动示例 ----------
    cout << "\n=== std::string 移动 ===" << endl;
    string s1 = "Hello World";
    string s2 = move(s1);     // 移动
    cout << "s1 空: " << boolalpha << s1.empty() << endl;  // true
    cout << "s2: " << s2 << endl;

    cout << "\n=== 程序结束 ===" << endl;
    return 0;
}
