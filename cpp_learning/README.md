# C++ 学习之路：从基础到高级

> 本目录包含系统化的 C++ 学习材料，从入门基础到 C++11/14/17 高级特性，并附带两个综合练习项目。

---

## 目录结构

```
cpp_learning/
├── 00_basics/              # C++ 基础知识
│   ├── 01_hello_world.cpp
│   ├── 02_variables_and_operators.cpp
│   ├── 03_control_flow.cpp
│   ├── 04_functions.cpp
│   ├── 05_arrays_strings_refs.cpp
│   └── 06_pointers.cpp
│
├── 01_intermediate/        # C++ 中级进阶
│   ├── 01_oop_basics.cpp
│   ├── 02_inheritance_polymorphism.cpp
│   ├── 03_templates.cpp
│   ├── 04_stl_containers.cpp
│   └── 05_exceptions_and_fileio.cpp
│
├── 02_advanced_cpp11/      # 现代 C++（C++11 及以上）
│   ├── 01_modern_cpp_features.cpp
│   ├── 02_lambda_and_functional.cpp
│   ├── 03_smart_pointers.cpp
│   ├── 04_move_semantics.cpp
│   └── 05_concurrency.cpp
│
└── 03_projects/            # 综合练习项目
    ├── student_management/ # 学生成绩管理系统
    └── bank_system/        # 银行账户管理系统
```

---

## 学习路径

### 第一阶段：基础（推荐 1-2 周）

| 文件 | 核心知识点 |
|------|-----------|
| `01_hello_world.cpp` | `#include`、`cout`、`cin`、`main()` 函数 |
| `02_variables_and_operators.cpp` | 数据类型、`sizeof`、算术/位运算符、类型转换 |
| `03_control_flow.cpp` | `if/else`、`switch`、`while`、`for`、`break/continue` |
| `04_functions.cpp` | 函数声明、默认参数、重载、递归、传值/引用/指针 |
| `05_arrays_strings_refs.cpp` | 数组、`std::vector`、`std::string`、二维数组 |
| `06_pointers.cpp` | 指针算术、`const` 指针、`new/delete`、函数指针 |

### 第二阶段：中级（推荐 2-3 周）

| 文件 | 核心知识点 |
|------|-----------|
| `01_oop_basics.cpp` | 类、访问修饰符、构造/析构函数、`this` 指针、运算符重载 |
| `02_inheritance_polymorphism.cpp` | 继承、虚函数、纯虚函数、抽象类、`dynamic_cast` |
| `03_templates.cpp` | 函数模板、类模板、模板特化、变参模板 |
| `04_stl_containers.cpp` | `vector/list/map/set/unordered_map`、`stack/queue` |
| `05_exceptions_and_fileio.cpp` | `try/catch/throw`、自定义异常、`ifstream/ofstream` |

### 第三阶段：现代 C++11+（推荐 3-4 周）

| 文件 | 核心知识点 |
|------|-----------|
| `01_modern_cpp_features.cpp` | `auto`、`constexpr`、`enum class`、`nullptr`、委托构造 |
| `02_lambda_and_functional.cpp` | Lambda 表达式、捕获列表、`std::function`、闭包 |
| `03_smart_pointers.cpp` | `unique_ptr`、`shared_ptr`、`weak_ptr`、循环引用 |
| `04_move_semantics.cpp` | 右值引用、移动构造、`std::move`、完美转发、RVO |
| `05_concurrency.cpp` | `std::thread`、`mutex`、`condition_variable`、`async/future` |

### 第四阶段：综合项目（推荐 1-2 周）

| 项目 | 技术栈 |
|------|-------|
| **学生成绩管理系统** | OOP + STL + 文件 I/O + 异常 + Lambda + 统计分析 |
| **银行账户管理系统** | 继承多态 + 智能指针 + 移动语义 + 模板 + Lambda 过滤 |

---

## 编译方式

### 方式一：单文件编译（快速上手）

```bash
# C++11 标准（适用于大多数文件）
g++ -std=c++11 -Wall -o output 00_basics/01_hello_world.cpp && ./output

# C++17 标准（推荐）
g++ -std=c++17 -Wall -o output 00_basics/01_hello_world.cpp && ./output

# 并发程序需要链接 pthread
g++ -std=c++11 -pthread -o concurrency 02_advanced_cpp11/05_concurrency.cpp && ./concurrency
```

### 方式二：CMake 批量编译

```bash
cd cpp_learning
mkdir build && cd build
cmake ..
make -j4          # 并行编译
./01_hello_world  # 运行
```

---

## 各章节重点总结

### 🔵 基础篇重点
- **指针**是 C++ 的精髓，务必搞清楚 `*`（解引用）和 `&`（取地址）的区别
- **函数传参**：值传递不修改原变量，引用传递可以修改
- `new`/`delete` 必须配对，否则内存泄漏

### 🟡 中级篇重点
- OOP 三大特性：**封装**（class）、**继承**（:`public`）、**多态**（`virtual`）
- 虚析构函数：用基类指针管理派生类时**必须**声明为 `virtual ~Base()`
- STL 首选：`vector`（顺序）、`unordered_map`（查找）、`set`（去重+排序）

### 🔴 高级篇重点
- **智能指针**：用 `unique_ptr` 代替裸指针，杜绝内存泄漏
- **移动语义**：大对象传参/返回时用 `std::move`，减少深拷贝
- **Lambda**：比函数对象更简洁，与 STL 算法完美结合
- **并发**：注意数据竞争，优先用 `atomic`，复杂情况用 `mutex`

---

## 常用 C++11/14/17 特性速查

```cpp
// auto 类型推断
auto x = 42;
auto it = vec.begin();

// 范围 for 循环
for (const auto& item : container) { ... }

// Lambda
auto fn = [capture](params) -> return_type { body };

// 智能指针
auto p = make_unique<MyClass>(args...);    // C++14
auto s = make_shared<MyClass>(args...);

// 移动语义
std::move(lvalue);          // 转为右值，触发移动
std::forward<T>(arg);       // 完美转发

// 初始化
vector<int> v{1, 2, 3};    // 统一初始化
auto [a, b] = pair{1, 2};  // C++17 结构化绑定

// constexpr 编译期计算
constexpr int N = 10;
constexpr int factorial(int n) { return n <= 1 ? 1 : n * factorial(n-1); }

// 并发
auto fut = std::async(std::launch::async, [](){ return 42; });
int result = fut.get();
```

---

## 学习建议

1. **动手编译运行**每一个示例，观察输出结果
2. **修改参数**，看看会发生什么（边学边实验）
3. 基础篇中的**指针**和中级篇的**多态**是两大难点，多花时间
4. 高级篇的**移动语义**是 C++11 性能优化的核心，深入理解它
5. 完成项目时，尝试**自己添加新功能**（如排序方式、新账户类型等）

---

*持续更新中，欢迎提 Issue 和 PR！*
