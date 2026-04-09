/**
 * 05_arrays_strings.cpp
 * C++ 入门：数组与字符串
 * 演示 C 数组、多维数组、std::string 的常用操作
 */

#include <iostream>
#include <string>
#include <algorithm>  // sort, reverse
using namespace std;

int main() {
    // ---------- 一维数组 ----------
    cout << "=== 一维数组 ===" << endl;
    int arr[5] = {10, 20, 30, 40, 50};
    int len = sizeof(arr) / sizeof(arr[0]);
    for (int i = 0; i < len; i++) {
        cout << "arr[" << i << "] = " << arr[i] << endl;
    }

    // ---------- 范围 for（C++11） ----------
    cout << "\n=== 范围 for 遍历数组 ===" << endl;
    for (int val : arr) {
        cout << val << " ";
    }
    cout << endl;

    // ---------- 排序 ----------
    int nums[] = {5, 3, 8, 1, 9, 2};
    int numsLen = sizeof(nums) / sizeof(nums[0]);
    sort(nums, nums + numsLen);
    cout << "\n=== 排序后 ===" << endl;
    for (int v : nums) cout << v << " ";
    cout << endl;

    // ---------- 二维数组 ----------
    cout << "\n=== 二维数组（矩阵） ===" << endl;
    int matrix[3][3] = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}
    };
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            cout << matrix[i][j] << " ";
        }
        cout << endl;
    }

    // ---------- std::string ----------
    cout << "\n=== std::string ===" << endl;
    string s1 = "Hello";
    string s2 = "World";

    // 拼接
    string s3 = s1 + ", " + s2 + "!";
    cout << "拼接: " << s3 << endl;

    // 长度
    cout << "长度: " << s3.size() << endl;

    // 子串
    cout << "子串(0,5): " << s3.substr(0, 5) << endl;

    // 查找
    size_t pos = s3.find("World");
    if (pos != string::npos) {
        cout << "\"World\" 在位置: " << pos << endl;
    }

    // 替换
    s3.replace(7, 5, "C++");
    cout << "替换后: " << s3 << endl;

    // 转大写
    string upper = s1;
    for (char& ch : upper) ch = toupper(ch);
    cout << "转大写: " << upper << endl;

    // 反转
    string rev = s1;
    reverse(rev.begin(), rev.end());
    cout << "反转: " << rev << endl;

    // ---------- 字符串转数字 / 数字转字符串 ----------
    cout << "\n=== 字符串转换 ===" << endl;
    string numStr = "42";
    int num = stoi(numStr);
    cout << "stoi(\"42\") = " << num << endl;

    double pi = 3.14159;
    string piStr = to_string(pi);
    cout << "to_string(3.14159) = " << piStr << endl;

    return 0;
}
