cmake_minimum_required(VERSION 3.14)
project(CppLearning CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 通用编译选项
add_compile_options(-Wall -Wextra -Wpedantic)

# ─── 基础模块 ────────────────────────────────────────────────────
add_executable(01_hello_world         00_basics/01_hello_world.cpp)
add_executable(02_variables           00_basics/02_variables_and_operators.cpp)
add_executable(03_control_flow        00_basics/03_control_flow.cpp)
add_executable(04_functions           00_basics/04_functions.cpp)
add_executable(05_arrays_strings      00_basics/05_arrays_strings_refs.cpp)
add_executable(06_pointers            00_basics/06_pointers.cpp)

# ─── 中级模块 ────────────────────────────────────────────────────
add_executable(07_oop_basics          01_intermediate/01_oop_basics.cpp)
add_executable(08_inheritance         01_intermediate/02_inheritance_polymorphism.cpp)
add_executable(09_templates           01_intermediate/03_templates.cpp)
add_executable(10_stl                 01_intermediate/04_stl_containers.cpp)
add_executable(11_exceptions_fileio   01_intermediate/05_exceptions_and_fileio.cpp)

# ─── 高级模块 ────────────────────────────────────────────────────
add_executable(12_modern_cpp          02_advanced_cpp11/01_modern_cpp_features.cpp)
add_executable(13_lambda              02_advanced_cpp11/02_lambda_and_functional.cpp)
add_executable(14_smart_ptrs          02_advanced_cpp11/03_smart_pointers.cpp)
add_executable(15_move_semantics      02_advanced_cpp11/04_move_semantics.cpp)

# 多线程需要链接 pthread
find_package(Threads REQUIRED)
add_executable(16_concurrency         02_advanced_cpp11/05_concurrency.cpp)
target_link_libraries(16_concurrency Threads::Threads)

# ─── 综合项目 ────────────────────────────────────────────────────
add_executable(student_management     03_projects/student_management/student_management.cpp)
add_executable(bank_system            03_projects/bank_system/bank_system.cpp)
