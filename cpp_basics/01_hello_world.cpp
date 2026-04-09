/**
 * 01_hello_world.cpp
 * C++ 入门：Hello World 程序
 * 演示最基础的 C++ 程序结构：头文件引用、main 函数、标准输出
 */

#include <iostream>  // 标准输入输出流头文件

int main() {
    std::cout << "Hello, World!" << std::endl;

    // 使用 using namespace std 简化写法
    using namespace std;
    cout << "欢迎学习 C++！" << endl;

    return 0;  // 程序正常结束，返回 0
}
