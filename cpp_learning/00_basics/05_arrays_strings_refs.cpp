/**
 * 【C++ 基础 - 第5课：数组、字符串与引用】
 *
 * 知识点：
 * 1. 原生数组（C-style array）：声明、遍历、越界风险
 * 2. std::array（C++11）：固定大小，有边界检查
 * 3. std::vector：动态数组，最常用的容器
 * 4. C 风格字符串 (char[]) vs std::string
 * 5. string 常用操作：拼接、查找、截取、比较
 * 6. 引用（reference）：别名，不可重新绑定
 * 7. 二维数组 & vector<vector<int>>
 *
 * 练习：
 *   - 统计字符串中每个字母出现的次数
 *   - 实现矩阵转置
 */

#include <iostream>
#include <array>     // std::array（C++11）
#include <vector>
#include <string>
#include <algorithm> // sort, find, transform

using namespace std;

int main() {
    // ─── 1. 原生数组 ──────────────────────────────────────────────
    cout << "=== 原生数组 ===" << endl;
    int scores[5] = {85, 92, 78, 96, 67};
    int n = sizeof(scores) / sizeof(scores[0]);  // 计算元素个数

    cout << "成绩：";
    for (int i = 0; i < n; i++) cout << scores[i] << " ";
    cout << endl;
    cout << "元素个数：" << n << endl;

    // ─── 2. std::array（C++11）────────────────────────────────────
    cout << "\n=== std::array ===" << endl;
    array<int, 5> arr = {3, 1, 4, 1, 5};
    arr.at(2) = 99;  // at() 有边界检查，越界抛异常
    cout << "arr: ";
    for (int x : arr) cout << x << " ";
    cout << "\nsize = " << arr.size() << endl;

    sort(arr.begin(), arr.end());  // 排序
    cout << "排序后: ";
    for (int x : arr) cout << x << " ";
    cout << endl;

    // ─── 3. std::vector ───────────────────────────────────────────
    cout << "\n=== std::vector ===" << endl;
    vector<int> v = {10, 20, 30, 40, 50};

    v.push_back(60);      // 末尾追加
    v.insert(v.begin() + 2, 25);  // 在索引2处插入 25
    v.erase(v.begin() + 4);       // 删除索引4元素

    cout << "vector: ";
    for (int x : v) cout << x << " ";
    cout << "\nsize=" << v.size() << " capacity=" << v.capacity() << endl;

    // 迭代器遍历
    cout << "迭代器遍历：";
    for (auto it = v.begin(); it != v.end(); ++it) cout << *it << " ";
    cout << endl;

    // ─── 4. std::string 常用操作 ──────────────────────────────────
    cout << "\n=== std::string ===" << endl;
    string s1 = "Hello";
    string s2 = " World";
    string s3 = s1 + s2;          // 拼接
    cout << "拼接：" << s3 << endl;
    cout << "长度：" << s3.length() << endl;
    cout << "大写H位置：" << s3.find('W') << endl;  // 找字符

    string sub = s3.substr(6, 5);   // 从索引6截取5个字符
    cout << "截取：" << sub << endl;

    s3.replace(6, 5, "C++");        // 替换
    cout << "替换：" << s3 << endl;

    // 字符串转数字
    string num_str = "42";
    int    num_int = stoi(num_str);   // string to int
    double num_dbl = stod("3.14");    // string to double
    cout << "stoi(\"42\") = " << num_int << endl;
    cout << "stod(\"3.14\") = " << num_dbl << endl;

    // 数字转字符串
    string from_num = to_string(12345);
    cout << "to_string(12345) = " << from_num << endl;

    // 字符串比较
    string a = "apple", b = "banana";
    cout << "\na < b ? " << (a < b ? "是" : "否") << endl;

    // 逐字符遍历
    string word = "Hello";
    cout << "逐字符：";
    for (char c : word) cout << c << " ";
    cout << endl;

    // transform 转大写
    string upper = word;
    transform(upper.begin(), upper.end(), upper.begin(), ::toupper);
    cout << "转大写：" << upper << endl;

    // ─── 5. 引用 ──────────────────────────────────────────────────
    cout << "\n=== 引用 ===" << endl;
    int  x   = 100;
    int& ref = x;     // ref 是 x 的别名
    ref = 200;        // 修改 ref 等同于修改 x
    cout << "x = " << x << " (通过引用修改)" << endl;

    // ─── 6. 二维数组 ──────────────────────────────────────────────
    cout << "\n=== 二维数组：3×3 矩阵 ===" << endl;
    int mat[3][3] = {{1,2,3},{4,5,6},{7,8,9}};

    cout << "原矩阵：" << endl;
    for (int r = 0; r < 3; r++) {
        for (int c = 0; c < 3; c++) cout << mat[r][c] << " ";
        cout << endl;
    }

    // 矩阵转置
    int trans[3][3];
    for (int r = 0; r < 3; r++)
        for (int c = 0; c < 3; c++)
            trans[c][r] = mat[r][c];

    cout << "转置矩阵：" << endl;
    for (int r = 0; r < 3; r++) {
        for (int c = 0; c < 3; c++) cout << trans[r][c] << " ";
        cout << endl;
    }

    // ─── 练习：统计字母频率 ───────────────────────────────────────
    cout << "\n=== 练习：统计字母频率 ===" << endl;
    string text = "Hello World CPlusPlus";
    int freq[26] = {};  // 初始化全零
    for (char c : text) {
        if (isalpha(c)) freq[tolower(c) - 'a']++;
    }
    cout << "字符串：\"" << text << "\"" << endl;
    for (int i = 0; i < 26; i++) {
        if (freq[i] > 0)
            cout << (char)('a' + i) << ": " << freq[i] << endl;
    }

    // ─── 练习：vector<vector> 实现动态矩阵 ──────────────────────
    cout << "\n=== vector<vector<int>> 矩阵 ===" << endl;
    int rows = 3, cols = 4;
    vector<vector<int>> matrix(rows, vector<int>(cols, 0));

    // 填入数据
    for (int r = 0; r < rows; r++)
        for (int c = 0; c < cols; c++)
            matrix[r][c] = r * cols + c + 1;

    cout << "动态矩阵：" << endl;
    for (auto& row : matrix) {
        for (int val : row) cout << val << "\t";
        cout << endl;
    }

    return 0;
}
