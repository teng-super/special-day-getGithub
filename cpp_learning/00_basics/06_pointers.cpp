/**
 * 【C++ 基础 - 第6课：指针深入讲解】
 *
 * 知识点：
 * 1. 指针本质：存储变量内存地址的变量
 * 2. 取地址 & 与解引用 *
 * 3. 指针算术（pointer arithmetic）
 * 4. 指针与数组的关系
 * 5. 指针的指针（二级指针）
 * 6. 函数指针
 * 7. 动态内存分配：new / delete
 * 8. 常见陷阱：野指针、内存泄漏、悬空指针
 * 9. nullptr（C++11）替代 NULL/0
 *
 * 练习：
 *   - 用 new[] 动态创建数组并排序，最后 delete[]
 */

#include <iostream>
#include <algorithm>  // sort
#include <cstring>    // memset

using namespace std;

// 函数指针示例：接受两个 int 返回 int 的函数指针类型
typedef int (*BinaryOp)(int, int);

int add_fn(int a, int b) { return a + b; }
int sub_fn(int a, int b) { return a - b; }
int mul_fn(int a, int b) { return a * b; }

// 接受函数指针作参数
int apply(BinaryOp op, int a, int b) {
    return op(a, b);
}

int main() {
    // ─── 1. 基本指针操作 ──────────────────────────────────────────
    cout << "=== 基本指针 ===" << endl;
    int  x  = 42;
    int* px = &x;           // px 存储 x 的地址

    cout << "x 的值：" << x       << endl;
    cout << "x 的地址：" << &x    << endl;
    cout << "px 存储的地址：" << px << endl;
    cout << "*px（解引用）：" << *px << endl;

    *px = 100;  // 通过指针修改变量
    cout << "通过 *px=100 后，x = " << x << endl;

    // ─── 2. 指针算术 ──────────────────────────────────────────────
    cout << "\n=== 指针算术 ===" << endl;
    int arr[] = {10, 20, 30, 40, 50};
    int* p = arr;  // 指向数组首元素，等价于 &arr[0]

    cout << "p  -> arr[0] = " << *p        << endl;
    cout << "p+1 -> arr[1] = " << *(p + 1) << endl;
    cout << "p+2 -> arr[2] = " << *(p + 2) << endl;

    // 指针遍历数组
    cout << "指针遍历：";
    for (int* q = arr; q < arr + 5; q++) cout << *q << " ";
    cout << endl;

    // ─── 3. const 指针 ────────────────────────────────────────────
    cout << "\n=== const 指针 ===" << endl;
    int a = 5, b = 10;

    // 指向常量的指针：不能通过指针修改值
    const int* pc1 = &a;
    // *pc1 = 99;  // ❌ 编译错误
    pc1 = &b;     // ✅ 可以改变指向
    cout << "const int* 指向 b=" << *pc1 << endl;

    // 常量指针：指向不变，但值可改
    int* const pc2 = &a;
    *pc2 = 99;   // ✅
    // pc2 = &b; // ❌
    cout << "int* const：a=" << a << endl;

    // 指向常量的常量指针：都不能改
    const int* const pc3 = &b;
    cout << "const int* const：b=" << *pc3 << endl;

    // ─── 4. 二级指针（指针的指针）────────────────────────────────
    cout << "\n=== 二级指针 ===" << endl;
    int   val    = 42;
    int*  pval   = &val;
    int** ppval  = &pval;

    cout << "val   = " << val     << endl;
    cout << "*pval = " << *pval   << endl;
    cout << "**ppval = " << **ppval << endl;

    **ppval = 888;
    cout << "**ppval=888 后，val = " << val << endl;

    // ─── 5. 函数指针 ──────────────────────────────────────────────
    cout << "\n=== 函数指针 ===" << endl;
    BinaryOp ops[] = {add_fn, sub_fn, mul_fn};
    const char* names[] = {"加", "减", "乘"};
    int m = 6, n = 4;
    for (int i = 0; i < 3; i++) {
        cout << m << " " << names[i] << " " << n << " = " << apply(ops[i], m, n) << endl;
    }

    // ─── 6. 动态内存分配：new / delete ───────────────────────────
    cout << "\n=== new / delete ===" << endl;

    // 单个变量
    int* dyn = new int(99);
    cout << "动态分配 int: " << *dyn << " 地址：" << dyn << endl;
    delete dyn;
    dyn = nullptr;  // 置 nullptr，防止悬空指针

    // 动态数组
    int size;
    cout << "请输入数组大小：";
    cin >> size;

    int* dynArr = new int[size];
    // 用随机数填充
    srand(42);
    cout << "随机填充：";
    for (int i = 0; i < size; i++) {
        dynArr[i] = rand() % 100;
        cout << dynArr[i] << " ";
    }
    cout << endl;

    sort(dynArr, dynArr + size);

    cout << "排序后：";
    for (int i = 0; i < size; i++) cout << dynArr[i] << " ";
    cout << endl;

    delete[] dynArr;   // 数组必须用 delete[]
    dynArr = nullptr;

    // ─── 7. 常见陷阱演示（注释掉危险代码，仅说明）────────────────
    cout << "\n=== 常见陷阱说明 ===" << endl;
    cout << "1. 野指针：未初始化的指针，指向随机地址 -> 未定义行为" << endl;
    cout << "   修复：声明时初始化 int* ptr = nullptr;" << endl;
    cout << "2. 内存泄漏：new 了但没 delete -> 内存无法回收" << endl;
    cout << "   修复：配对使用 new/delete，或用智能指针（C++11）" << endl;
    cout << "3. 悬空指针：delete 后仍使用 -> 未定义行为" << endl;
    cout << "   修复：delete 后立即置 nullptr" << endl;
    cout << "4. 数组越界：访问 arr[size] -> 未定义行为" << endl;
    cout << "   修复：使用 std::vector 或 std::array，有 .at() 检查" << endl;

    // ─── 8. nullptr（C++11）──────────────────────────────────────
    cout << "\n=== nullptr ===" << endl;
    int* ptr = nullptr;
    if (ptr == nullptr) cout << "ptr 是空指针（nullptr）" << endl;
    // 注意：nullptr 有类型，是 nullptr_t，不是整数 0
    // 可以赋给任意指针类型，但不能隐式转为 int

    return 0;
}
