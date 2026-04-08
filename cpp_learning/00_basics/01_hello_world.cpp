/**
 * 【C++ 基础 - 第1课：Hello World & 基本输入输出】
 *
 * 知识点：
 * 1. #include 预处理指令：引入标准库头文件
 * 2. using namespace std：使用标准命名空间，避免写 std::cout
 * 3. int main()：程序入口函数，返回 0 表示正常退出
 * 4. cout / cin：标准输出 / 标准输入流对象
 * 5. endl：换行并刷新输出缓冲区（也可用 '\n'，速度更快）
 *
 * 编译方法：
 *   g++ -std=c++11 -o hello 01_hello_world.cpp
 *   ./hello
 */

#include <iostream>
#include <string>

using namespace std;

int main() {
    // 输出到控制台
    cout << "Hello, World!" << endl;
    cout << "欢迎学习 C++！" << endl;

    // 基本变量与输出格式
    int    age  = 20;
    double gpa  = 3.85;
    string name = "小明";

    cout << "姓名：" << name << endl;
    cout << "年龄：" << age  << endl;
    cout << "GPA：" << gpa  << endl;

    // 读取用户输入
    cout << "\n请输入您的名字：";
    string input_name;
    cin >> input_name;
    cout << "你好，" << input_name << "！欢迎来到 C++ 的世界！" << endl;

    return 0;
}
