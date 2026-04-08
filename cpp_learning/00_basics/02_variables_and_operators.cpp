/**
 * 【C++ 基础 - 第2课：变量、数据类型与运算符】
 *
 * 知识点：
 * 1. 基本数据类型：int, long long, float, double, char, bool
 * 2. 类型占用字节：sizeof 运算符
 * 3. 类型转换：隐式转换 vs 显式转换 (static_cast)
 * 4. 常量：const 关键字、#define 宏（不推荐）
 * 5. 运算符：算术、关系、逻辑、位运算、赋值、自增自减
 * 6. 整数溢出：了解上下界 (limits 头文件)
 *
 * 练习：
 *   - 输入两个整数，输出它们的加减乘除和取余结果
 *   - 体会整数除法与浮点数除法的区别
 */

#include <iostream>
#include <limits>   // numeric_limits
#include <climits>  // INT_MAX, INT_MIN

using namespace std;

int main() {
    // ─── 1. 基本类型大小 ─────────────────────────────────────────
    cout << "=== 数据类型大小 ===" << endl;
    cout << "bool:      " << sizeof(bool)      << " 字节" << endl;
    cout << "char:      " << sizeof(char)      << " 字节" << endl;
    cout << "int:       " << sizeof(int)       << " 字节" << endl;
    cout << "long long: " << sizeof(long long) << " 字节" << endl;
    cout << "float:     " << sizeof(float)     << " 字节" << endl;
    cout << "double:    " << sizeof(double)    << " 字节" << endl;

    // ─── 2. 整数类型范围 ─────────────────────────────────────────
    cout << "\n=== int 的取值范围 ===" << endl;
    cout << "INT_MAX = " << INT_MAX << endl;
    cout << "INT_MIN = " << INT_MIN << endl;
    cout << "long long max = " << numeric_limits<long long>::max() << endl;

    // ─── 3. 常量 ─────────────────────────────────────────────────
    const double PI    = 3.14159265358979;
    const int    DAYS  = 365;
    cout << "\nPI = " << PI << ", 一年 = " << DAYS << " 天" << endl;

    // ─── 4. 运算符 ───────────────────────────────────────────────
    int a = 17, b = 5;
    cout << "\n=== 算术运算符（a=17, b=5）===" << endl;
    cout << "a + b = " << a + b << endl;
    cout << "a - b = " << a - b << endl;
    cout << "a * b = " << a * b << endl;
    cout << "a / b = " << a / b << "  （整数除法，舍去小数）" << endl;
    cout << "a % b = " << a % b << "  （取余）" << endl;

    // 浮点除法
    cout << "\n=== 浮点除法 ===" << endl;
    cout << "(double)a / b = " << (double)a / b << endl;
    cout << "static_cast：" << static_cast<double>(a) / b << endl;

    // ─── 5. 关系 & 逻辑运算符 ────────────────────────────────────
    cout << "\n=== 关系与逻辑运算符 ===" << endl;
    bool x = true, y = false;
    cout << "x && y = " << (x && y) << endl;  // AND
    cout << "x || y = " << (x || y) << endl;  // OR
    cout << "!x     = " << (!x)     << endl;  // NOT
    cout << "a > b  = " << (a > b)  << endl;

    // ─── 6. 位运算 ───────────────────────────────────────────────
    cout << "\n=== 位运算（a=17=0b10001, b=5=0b00101）===" << endl;
    cout << "a & b  = " << (a & b)  << endl;  // 按位与
    cout << "a | b  = " << (a | b)  << endl;  // 按位或
    cout << "a ^ b  = " << (a ^ b)  << endl;  // 按位异或
    cout << "a << 1 = " << (a << 1) << "  （左移1位 = 乘2）" << endl;
    cout << "a >> 1 = " << (a >> 1) << "  （右移1位 = 除2）" << endl;

    // ─── 7. 自增自减 ─────────────────────────────────────────────
    cout << "\n=== 自增自减 ===" << endl;
    int n = 5;
    cout << "n   = " << n   << endl;
    cout << "n++ = " << n++ << "  （先用后增）" << endl;
    cout << "n   = " << n   << "  （已变为6）" << endl;
    cout << "++n = " << ++n << "  （先增后用）" << endl;

    // ─── 练习：输入两整数做四则运算 ──────────────────────────────
    cout << "\n=== 练习 ===" << endl;
    cout << "请输入两个整数：";
    int p, q;
    cin >> p >> q;
    cout << p << " + " << q << " = " << p + q << endl;
    cout << p << " - " << q << " = " << p - q << endl;
    cout << p << " * " << q << " = " << p * q << endl;
    if (q != 0) {
        cout << p << " / " << q << " = " << (double)p / q << endl;
        cout << p << " % " << q << " = " << p % q << endl;
    } else {
        cout << "除数不能为 0！" << endl;
    }

    return 0;
}
