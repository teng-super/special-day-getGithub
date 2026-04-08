/**
 * 【C++ 基础 - 第3课：控制流（条件 & 循环）】
 *
 * 知识点：
 * 1. if / else if / else 条件语句
 * 2. switch / case 多分支选择
 * 3. while 循环
 * 4. do-while 循环（至少执行一次）
 * 5. for 循环
 * 6. break（跳出循环）/ continue（跳过本次迭代）
 * 7. 三元运算符 condition ? a : b
 *
 * 练习：
 *   - 猜数字游戏：随机生成 1-100 的整数，用户不断猜测直到猜中
 */

#include <iostream>
#include <cstdlib>  // rand, srand
#include <ctime>    // time

using namespace std;

// 判断成绩等级
string grade(int score) {
    if      (score >= 90) return "A（优秀）";
    else if (score >= 80) return "B（良好）";
    else if (score >= 70) return "C（中等）";
    else if (score >= 60) return "D（及格）";
    else                  return "F（不及格）";
}

int main() {
    // ─── 1. if / else ────────────────────────────────────────────
    cout << "=== 成绩等级判定 ===" << endl;
    int scores[] = {95, 82, 71, 63, 45};
    for (int s : scores) {
        cout << "分数 " << s << " => " << grade(s) << endl;
    }

    // ─── 2. switch / case ────────────────────────────────────────
    cout << "\n=== switch：星期几 ===" << endl;
    int day = 3;
    switch (day) {
        case 1: cout << "星期一" << endl; break;
        case 2: cout << "星期二" << endl; break;
        case 3: cout << "星期三" << endl; break;
        case 4: cout << "星期四" << endl; break;
        case 5: cout << "星期五" << endl; break;
        case 6: cout << "星期六" << endl; break;
        case 7: cout << "星期日" << endl; break;
        default: cout << "无效输入" << endl;
    }

    // ─── 3. while 循环 ───────────────────────────────────────────
    cout << "\n=== while：累加 1 到 100 ===" << endl;
    int sum = 0, i = 1;
    while (i <= 100) {
        sum += i;
        i++;
    }
    cout << "1 + 2 + ... + 100 = " << sum << endl;

    // ─── 4. do-while 循环 ────────────────────────────────────────
    cout << "\n=== do-while：输入正整数 ===" << endl;
    int num;
    do {
        cout << "请输入一个正整数：";
        cin >> num;
        if (num <= 0) cout << "输入无效，请重试！" << endl;
    } while (num <= 0);
    cout << "你输入了：" << num << endl;

    // ─── 5. for 循环 & break / continue ─────────────────────────
    cout << "\n=== for 循环：打印 1-20 中的奇数 ===" << endl;
    for (int j = 1; j <= 20; j++) {
        if (j % 2 == 0) continue;  // 偶数跳过
        cout << j << " ";
        if (j == 15) {
            cout << "<-- 到 15 停止";
            break;  // 提前结束
        }
    }
    cout << endl;

    // ─── 6. 嵌套循环：九九乘法表 ─────────────────────────────────
    cout << "\n=== 九九乘法表 ===" << endl;
    for (int r = 1; r <= 9; r++) {
        for (int c = 1; c <= r; c++) {
            cout << c << "×" << r << "=" << c * r << "\t";
        }
        cout << endl;
    }

    // ─── 7. 三元运算符 ───────────────────────────────────────────
    cout << "\n=== 三元运算符：判断奇偶 ===" << endl;
    for (int k = 1; k <= 6; k++) {
        cout << k << " 是 " << (k % 2 == 0 ? "偶数" : "奇数") << endl;
    }

    // ─── 练习：猜数字游戏 ────────────────────────────────────────
    cout << "\n=== 练习：猜数字游戏（1-100）===" << endl;
    srand(static_cast<unsigned>(time(nullptr)));
    int secret = rand() % 100 + 1;
    int guess, attempts = 0;
    do {
        cout << "猜一个数字（1-100）：";
        cin >> guess;
        attempts++;
        if      (guess < secret) cout << "太小了！" << endl;
        else if (guess > secret) cout << "太大了！" << endl;
        else    cout << "恭喜！猜对了！答案是 " << secret
                     << "，共猜了 " << attempts << " 次。" << endl;
    } while (guess != secret);

    return 0;
}
