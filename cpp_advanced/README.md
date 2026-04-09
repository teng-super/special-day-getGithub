# 高级 C++ 知识（C++11/14/17/20）

本目录包含现代 C++（C++11 及以上）的高级特性详细示例，适合有一定 C++ 基础的学习者进阶。

## 文件列表

| 文件 | C++ 标准 | 主题 | 核心知识点 |
|------|----------|------|------------|
| [01_cpp11_core.cpp](01_cpp11_core.cpp) | C++11 | 核心语言特性 | `auto`、`decltype`、`nullptr`、范围 `for`、初始化列表、委托构造、`override`/`final`、`enum class` |
| [02_lambda.cpp](02_lambda.cpp) | C++11/14 | Lambda 表达式 | 语法、捕获方式（值/引用）、`mutable`、泛型 lambda、STL 算法中的 lambda、高阶函数 |
| [03_smart_pointers.cpp](03_smart_pointers.cpp) | C++11/14 | 智能指针 | `unique_ptr`、`shared_ptr`、`weak_ptr`、循环引用问题、`enable_shared_from_this`、自定义删除器 |
| [04_move_semantics.cpp](04_move_semantics.cpp) | C++11 | 移动语义与右值引用 | 左值/右值、右值引用 `&&`、移动构造/赋值、`std::move`、完美转发 `std::forward`、RVO/NRVO |
| [05_variadic_templates.cpp](05_variadic_templates.cpp) | C++11/17 | 可变参数模板 | 参数包、递归展开、fold expression（C++17）、`std::tuple`、结构化绑定 |
| [06_concurrency.cpp](06_concurrency.cpp) | C++11/14 | 多线程与并发 | `std::thread`、`mutex`/`lock_guard`、`condition_variable`、`atomic`、`future`/`promise`、`std::async` |
| [07_cpp17_features.cpp](07_cpp17_features.cpp) | C++17 | C++17 新特性 | 结构化绑定、`if` 初始化语句、`std::optional`、`std::variant`、`std::any`、`constexpr if`、`string_view`、并行算法 |
| [08_cpp20_features.cpp](08_cpp20_features.cpp) | C++20 | C++20 新特性 | Concepts、Ranges 管道视图、`span`、三路比较 `<=>`、`consteval`、`constinit` |
| [09_design_patterns.cpp](09_design_patterns.cpp) | C++11+ | 设计模式 | 单例、工厂、观察者、策略、RAII |
| [10_practice_projects.cpp](10_practice_projects.cpp) | C++17 | 综合练习项目 | 学生成绩管理、栈实现计算器、手写链表 |

## 编译运行

```bash
# C++11/14 特性
g++ -std=c++14 -Wall -o output 文件名.cpp

# C++17 特性
g++ -std=c++17 -Wall -o output 文件名.cpp

# C++20 特性（需要 GCC 10+ 或 Clang 11+）
g++ -std=c++20 -Wall -o output 文件名.cpp

# 多线程文件需链接 pthread
g++ -std=c++17 -Wall -pthread -o concurrency 06_concurrency.cpp

# 并行算法需链接 TBB（部分编译器/平台需要）
g++ -std=c++17 -Wall -o cpp17 07_cpp17_features.cpp -ltbb
```

## C++ 版本特性速查

### C++11 主要新特性
- `auto` 类型推导
- `nullptr` 替代 `NULL`
- 范围 `for` 循环
- Lambda 表达式
- 右值引用与移动语义
- 智能指针（`unique_ptr`、`shared_ptr`、`weak_ptr`）
- 可变参数模板
- 强类型枚举 `enum class`
- `constexpr` 常量表达式
- `std::thread` 多线程库
- `std::atomic` 原子操作
- 初始化列表
- `override` / `final`
- 委托构造函数

### C++14 主要新特性
- 泛型 lambda（参数用 `auto`）
- `make_unique`
- 返回类型推导
- 二进制字面量 `0b1010`
- 数字分隔符 `1'000'000`
- `std::exchange`

### C++17 主要新特性
- 结构化绑定 `auto [a, b] = pair`
- `if` / `switch` 初始化语句
- `std::optional`
- `std::variant`
- `std::any`
- `constexpr if`
- `std::string_view`
- 并行算法（`execution::par`）
- 折叠表达式
- `std::filesystem`

### C++20 主要新特性
- Concepts（概念）
- Ranges（范围库）
- 协程（Coroutines）
- Modules（模块）
- `std::span`
- 三路比较 `<=>`
- `consteval` / `constinit`
- `std::format`（类似 Python f-string）
- `std::jthread`（自动 join 的线程）
