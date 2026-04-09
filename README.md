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
<div align="center">

# 🎉 Congratulations teng-super! 🎉

<img src="https://octodex.github.com/images/welcometocat.png" height="200px" />

### 🌟 You've successfully completed the exercise! 🌟

## 🚀 Share Your Success!

**Show off your new skills and inspire others!**

<a href="https://twitter.com/intent/tweet?text=I%20just%20completed%20the%20%22Introduction%20to%20GitHub%22%20GitHub%20Skills%20hands-on%20exercise!%20%F0%9F%8E%89%0A%0Ahttps%3A%2F%2Fgithub.com%2Fteng-super%2Fspecial-day-getGithub%0A%0A%23GitHubSkills%20%23OpenSource%20%23GitHubLearn" target="_blank" rel="noopener noreferrer">
  <img src="https://img.shields.io/badge/Share%20on%20X-1da1f2?style=for-the-badge&logo=x&logoColor=white" alt="Share on X" />
</a>
<a href="https://bsky.app/intent/compose?text=I%20just%20completed%20the%20%22Introduction%20to%20GitHub%22%20GitHub%20Skills%20hands-on%20exercise!%20%F0%9F%8E%89%0A%0Ahttps%3A%2F%2Fgithub.com%2Fteng-super%2Fspecial-day-getGithub%0A%0A%23GitHubSkills%20%23OpenSource%20%23GitHubLearn" target="_blank" rel="noopener noreferrer">
  <img src="https://img.shields.io/badge/Share%20on%20Bluesky-0085ff?style=for-the-badge&logo=bluesky&logoColor=white" alt="Share on Bluesky" />
</a>
<a href="https://www.linkedin.com/feed/?shareActive=true&text=I%20just%20completed%20the%20%22Introduction%20to%20GitHub%22%20GitHub%20Skills%20hands-on%20exercise!%20%F0%9F%8E%89%0A%0Ahttps%3A%2F%2Fgithub.com%2Fteng-super%2Fspecial-day-getGithub%0A%0A%23GitHubSkills%20%23OpenSource%20%23GitHubLearn" target="_blank" rel="noopener noreferrer">
  <img src="https://img.shields.io/badge/Share%20on%20LinkedIn-0077b5?style=for-the-badge&logo=linkedin&logoColor=white" alt="Share on LinkedIn" />
</a>

### 🎯 What's Next?

**Keep the momentum going!**

[![](https://img.shields.io/badge/Return%20to%20Exercise-%E2%86%92-1f883d?style=for-the-badge&logo=github&labelColor=197935)](https://github.com/teng-super/special-day-getGithub/issues/2)
[![GitHub Skills](https://img.shields.io/badge/Explore%20GitHub%20Skills-000000?style=for-the-badge&logo=github&logoColor=white)](https://learn.github.com/skills))

*There's no better way to learn than building things!* 🚀

</div>

---

&copy; 2025 GitHub &bull; [Code of Conduct](https://www.contributor-covenant.org/version/2/1/code_of_conduct/code_of_conduct.md) &bull; [MIT License](https://gh.io/mit)

