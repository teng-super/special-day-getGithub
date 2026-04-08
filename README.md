# C++ 知识汇总

本仓库系统整理了 **C++ 基础入门**与**现代 C++（C++11/14/17/20）高级特性**，每个知识点均附有可运行的代码示例。

## 目录结构

```
.
├── cpp_basics/      # C++ 基础入门代码
└── cpp_advanced/    # 高级 C++（C++11 及以上）代码
```

## C++ 基础入门（[cpp_basics/](cpp_basics/)）

| # | 文件 | 主题 |
|---|------|------|
| 01 | [hello_world](cpp_basics/01_hello_world.cpp) | 程序结构与标准输出 |
| 02 | [data_types](cpp_basics/02_data_types.cpp) | 基本数据类型与类型转换 |
| 03 | [control_flow](cpp_basics/03_control_flow.cpp) | 控制流（if/switch/for/while）|
| 04 | [functions](cpp_basics/04_functions.cpp) | 函数：重载、默认参数、递归 |
| 05 | [arrays_strings](cpp_basics/05_arrays_strings.cpp) | 数组与 std::string |
| 06 | [pointers_references](cpp_basics/06_pointers_references.cpp) | 指针、引用、动态内存 |
| 07 | [oop_basics](cpp_basics/07_oop_basics.cpp) | 面向对象：类、继承、多态 |
| 08 | [templates](cpp_basics/08_templates.cpp) | 函数模板与类模板 |
| 09 | [stl_containers](cpp_basics/09_stl_containers.cpp) | STL 容器全览 |
| 10 | [file_io](cpp_basics/10_file_io.cpp) | 文件读写 |

## 高级 C++（[cpp_advanced/](cpp_advanced/)）

| # | 文件 | C++ 版本 | 主题 |
|---|------|----------|------|
| 01 | [cpp11_core](cpp_advanced/01_cpp11_core.cpp) | C++11 | auto/decltype/nullptr/enum class |
| 02 | [lambda](cpp_advanced/02_lambda.cpp) | C++11/14 | Lambda 表达式 |
| 03 | [smart_pointers](cpp_advanced/03_smart_pointers.cpp) | C++11/14 | unique_ptr/shared_ptr/weak_ptr |
| 04 | [move_semantics](cpp_advanced/04_move_semantics.cpp) | C++11 | 右值引用与移动语义 |
| 05 | [variadic_templates](cpp_advanced/05_variadic_templates.cpp) | C++11/17 | 可变参数模板与 fold expression |
| 06 | [concurrency](cpp_advanced/06_concurrency.cpp) | C++11/14 | 多线程、mutex、atomic、async |
| 07 | [cpp17_features](cpp_advanced/07_cpp17_features.cpp) | C++17 | optional/variant/any/string_view/并行算法 |
| 08 | [cpp20_features](cpp_advanced/08_cpp20_features.cpp) | C++20 | Concepts/Ranges/span/三路比较 |
| 09 | [design_patterns](cpp_advanced/09_design_patterns.cpp) | C++11+ | 单例/工厂/观察者/策略/RAII |
| 10 | [practice_projects](cpp_advanced/10_practice_projects.cpp) | C++17 | 综合练习：成绩管理、计算器、链表 |

## 快速编译

```bash
# 基础示例（C++17）
g++ -std=c++17 -Wall -o out cpp_basics/01_hello_world.cpp && ./out

# 多线程示例（需要 pthread）
g++ -std=c++17 -Wall -pthread -o conc cpp_advanced/06_concurrency.cpp && ./conc

# C++20 特性（需要 GCC 10+ 或 Clang 11+）
g++ -std=c++20 -Wall -o cpp20 cpp_advanced/08_cpp20_features.cpp && ./cpp20
```

## 学习路线

```
C++ 基础入门
  01 ~ 06  → 语言基础（类型、控制流、函数、指针）
  07       → 面向对象
  08 ~ 09  → 泛型与 STL
  10       → 文件 I/O
      ↓
现代 C++ 进阶
  C++11  → 01~04, 06（语言核心改进、智能指针、并发）
  C++11  → 02, 05（lambda、可变参数模板）
  C++17  → 07（optional/variant/并行算法）
  C++20  → 08（Concepts/Ranges）
      ↓
综合应用
  09 设计模式
  10 综合练习项目
```

---

&copy; 2025 GitHub &bull; [MIT License](https://gh.io/mit)
