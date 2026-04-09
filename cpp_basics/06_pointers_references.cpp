/**
 * 06_pointers_references.cpp
 * C++ 入门：指针与引用
 * 演示指针的声明/解引用/算术、引用、const 指针、指针数组
 */

#include <iostream>
using namespace std;

// 用指针交换两个值
void swapByPointer(int* a, int* b) {
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

// 用引用交换两个值（更安全、更直观）
void swapByRef(int& a, int& b) {
    int tmp = a;
    a = b;
    b = tmp;
}

int main() {
    // ---------- 指针基础 ----------
    cout << "=== 指针基础 ===" << endl;
    int x = 10;
    int* ptr = &x;         // ptr 保存 x 的地址
    cout << "x 的值:  " << x    << endl;
    cout << "x 的地址: " << &x   << endl;
    cout << "ptr 的值: " << ptr  << endl;  // 与 &x 相同
    cout << "*ptr:    " << *ptr << endl;  // 解引用，得到 x 的值

    *ptr = 20;   // 通过指针修改 x
    cout << "修改后 x = " << x << endl;

    // ---------- 指针算术 ----------
    cout << "\n=== 指针算术 ===" << endl;
    int arr[] = {10, 20, 30, 40, 50};
    int* p = arr;  // 指向数组首元素
    for (int i = 0; i < 5; i++) {
        cout << "*(p+" << i << ") = " << *(p + i) << endl;
    }

    // ---------- 空指针与 nullptr ----------
    cout << "\n=== nullptr ===" << endl;
    int* nullPtr = nullptr;
    if (nullPtr == nullptr) {
        cout << "nullPtr 是空指针" << endl;
    }

    // ---------- const 指针 ----------
    cout << "\n=== const 指针 ===" << endl;
    int val = 42;
    const int* cptr = &val;    // 指向 const int：不能通过 cptr 修改值
    int* const cptrConst = &val; // const 指针：指针本身不可变，但可修改所指值
    cout << "const int* 指向的值: " << *cptr << endl;
    *cptrConst = 100;
    cout << "int* const 修改后 val = " << val << endl;

    // ---------- 引用 ----------
    cout << "\n=== 引用 ===" << endl;
    int a = 5, b = 10;
    int& refA = a;     // refA 是 a 的别名
    refA = 99;
    cout << "通过引用修改后 a = " << a << endl;

    // ---------- 指针与引用的交换示例 ----------
    cout << "\n=== 值交换 ===" << endl;
    int m = 3, n = 7;
    cout << "交换前: m=" << m << ", n=" << n << endl;
    swapByPointer(&m, &n);
    cout << "指针交换后: m=" << m << ", n=" << n << endl;
    swapByRef(m, n);
    cout << "引用交换后: m=" << m << ", n=" << n << endl;

    // ---------- 动态内存分配 ----------
    cout << "\n=== 动态内存 (new / delete) ===" << endl;
    int* dynArr = new int[5];
    for (int i = 0; i < 5; i++) dynArr[i] = (i + 1) * 10;
    for (int i = 0; i < 5; i++) cout << dynArr[i] << " ";
    cout << endl;
    delete[] dynArr;   // 释放数组内存，避免内存泄漏
    dynArr = nullptr;

    return 0;
}
