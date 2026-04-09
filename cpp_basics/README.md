# C++ 基础入门知识汇总

本目录包含 C++ 基础知识的代码示例，适合 C++ 初学者逐步学习。

## 文件列表

| 文件 | 主题 | 知识点 |
|------|------|--------|
| [01_hello_world.cpp](01_hello_world.cpp) | Hello World | 程序结构、`#include`、`main`、`std::cout` |
| [02_data_types.cpp](02_data_types.cpp) | 基本数据类型 | `int`/`float`/`char`/`bool`、类型大小、类型转换 |
| [03_control_flow.cpp](03_control_flow.cpp) | 控制流 | `if/else`、`switch`、`for`、`while`、`do-while`、`break`/`continue`、九九乘法表 |
| [04_functions.cpp](04_functions.cpp) | 函数 | 声明/定义、默认参数、函数重载、内联函数、递归、lambda 简介 |
| [05_arrays_strings.cpp](05_arrays_strings.cpp) | 数组与字符串 | C 数组、多维数组、`std::string` 常用操作、字符串转换 |
| [06_pointers_references.cpp](06_pointers_references.cpp) | 指针与引用 | 指针基础、指针算术、`nullptr`、`const` 指针、引用、动态内存 |
| [07_oop_basics.cpp](07_oop_basics.cpp) | 面向对象基础 | 类/对象、封装、构造/析构、继承、多态、虚函数、静态成员 |
| [08_templates.cpp](08_templates.cpp) | 模板（泛型） | 函数模板、类模板、模板特化、非类型模板参数 |
| [09_stl_containers.cpp](09_stl_containers.cpp) | STL 容器 | `vector`、`list`、`deque`、`map`、`unordered_map`、`set`、`stack`、`queue`、`priority_queue` |
| [10_file_io.cpp](10_file_io.cpp) | 文件 I/O | `ofstream`/`ifstream`/`fstream`、文本文件、二进制文件、`stringstream` |

## 编译运行

每个文件均可独立编译：

```bash
# 使用 C++17 标准编译
g++ -std=c++17 -Wall -o output 文件名.cpp

# 示例
g++ -std=c++17 -Wall -o hello 01_hello_world.cpp
./hello
```

## 学习路线建议

```
01 Hello World
    ↓
02 数据类型
    ↓
03 控制流
    ↓
04 函数
    ↓
05 数组与字符串
    ↓
06 指针与引用
    ↓
07 面向对象
    ↓
08 模板
    ↓
09 STL 容器
    ↓
10 文件 I/O
```

学完基础后，可以进入 [../cpp_advanced/](../cpp_advanced/) 目录学习 C++11 及以上的现代 C++ 特性。
