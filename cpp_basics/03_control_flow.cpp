/**
 * 03_control_flow.cpp
 * C++ 入门：控制流
 * 演示 if/else、switch、for、while、do-while、break/continue
 */

#include <iostream>
using namespace std;

int main() {
    // ---------- if / else if / else ----------
    cout << "=== if / else ===" << endl;
    int score = 85;
    if (score >= 90) {
        cout << "优秀" << endl;
    } else if (score >= 75) {
        cout << "良好" << endl;
    } else if (score >= 60) {
        cout << "及格" << endl;
    } else {
        cout << "不及格" << endl;
    }

    // ---------- switch ----------
    cout << "\n=== switch ===" << endl;
    int day = 3;
    switch (day) {
        case 1: cout << "星期一" << endl; break;
        case 2: cout << "星期二" << endl; break;
        case 3: cout << "星期三" << endl; break;
        case 4: cout << "星期四" << endl; break;
        case 5: cout << "星期五" << endl; break;
        default: cout << "周末"   << endl; break;
    }

    // ---------- for 循环 ----------
    cout << "\n=== for 循环 ===" << endl;
    for (int i = 1; i <= 5; i++) {
        cout << i << " ";
    }
    cout << endl;

    // ---------- while 循环 ----------
    cout << "\n=== while 循环 ===" << endl;
    int n = 1;
    while (n <= 5) {
        cout << n << " ";
        n++;
    }
    cout << endl;

    // ---------- do-while 循环 ----------
    cout << "\n=== do-while 循环 ===" << endl;
    int m = 1;
    do {
        cout << m << " ";
        m++;
    } while (m <= 5);
    cout << endl;

    // ---------- break / continue ----------
    cout << "\n=== break / continue ===" << endl;
    for (int i = 1; i <= 10; i++) {
        if (i == 6) break;       // 遇到 6 停止
        if (i % 2 == 0) continue; // 跳过偶数
        cout << i << " ";
    }
    cout << endl;

    // ---------- 嵌套循环 + 标签跳出（goto 示例） ----------
    cout << "\n=== 九九乘法表 ===" << endl;
    for (int i = 1; i <= 9; i++) {
        for (int j = 1; j <= i; j++) {
            cout << j << "×" << i << "=" << i * j << "\t";
        }
        cout << endl;
    }

    return 0;
}
