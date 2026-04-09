/**
 * 02_data_types.cpp
 * C++ 入门：基本数据类型
 * 演示整型、浮点型、字符型、布尔型及类型大小
 */

#include <iostream>
#include <climits>   // 整型极值常量
#include <cfloat>    // 浮点型极值常量

using namespace std;

int main() {
    // ---------- 整型 ----------
    int a = 42;
    short s = 32767;
    long l = 2147483647L;
    long long ll = 9223372036854775807LL;
    unsigned int ui = 4294967295U;

    cout << "=== 整型 ===" << endl;
    cout << "int:       " << a  << "  (大小 " << sizeof(int)       << " 字节)" << endl;
    cout << "short:     " << s  << "  (大小 " << sizeof(short)     << " 字节)" << endl;
    cout << "long:      " << l  << "  (大小 " << sizeof(long)      << " 字节)" << endl;
    cout << "long long: " << ll << "  (大小 " << sizeof(long long) << " 字节)" << endl;
    cout << "unsigned int 最大值: " << UINT_MAX << endl;

    // ---------- 浮点型 ----------
    float  f  = 3.14f;
    double d  = 3.141592653589793;
    long double ld = 3.141592653589793238L;

    cout << "\n=== 浮点型 ===" << endl;
    cout << "float:       " << f  << "  (大小 " << sizeof(float)       << " 字节)" << endl;
    cout << "double:      " << d  << "  (大小 " << sizeof(double)      << " 字节)" << endl;
    cout << "long double: " << ld << "  (大小 " << sizeof(long double) << " 字节)" << endl;

    // ---------- 字符型 ----------
    char c = 'A';
    cout << "\n=== 字符型 ===" << endl;
    cout << "char: " << c << "  ASCII 值: " << (int)c << endl;
    cout << "char 大小: " << sizeof(char) << " 字节" << endl;

    // ---------- 布尔型 ----------
    bool flag = true;
    cout << "\n=== 布尔型 ===" << endl;
    cout << "bool true:  " << flag << endl;
    cout << "bool false: " << !flag << endl;
    cout << "boolalpha: " << boolalpha << flag << " / " << !flag << endl;

    // ---------- 类型转换 ----------
    cout << "\n=== 类型转换 ===" << endl;
    double result = (double)a / 5;     // C 风格强制转换
    double result2 = static_cast<double>(a) / 5;  // C++ 风格转换（推荐）
    cout << "42 / 5 (int 除法) = " << a / 5 << endl;
    cout << "42 / 5 (double)   = " << result2 << endl;

    return 0;
}
