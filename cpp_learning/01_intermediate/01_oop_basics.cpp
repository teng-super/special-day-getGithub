/**
 * 【C++ 中级 - 第1课：面向对象编程（OOP）基础】
 *
 * 知识点：
 * 1. 类（class）与结构体（struct）的区别
 *    - class：成员默认 private；struct：成员默认 public
 * 2. 访问修饰符：public / protected / private
 * 3. 构造函数（Constructor）与析构函数（Destructor）
 * 4. 成员初始化列表（更高效的初始化方式）
 * 5. this 指针
 * 6. 拷贝构造函数 & 赋值运算符（Rule of Three）
 * 7. static 成员变量与成员函数
 * 8. const 成员函数（不修改对象状态）
 * 9. 友元函数（friend）
 * 10. 运算符重载
 *
 * 练习：
 *   - 实现一个 Complex（复数）类，支持加减乘和输出
 */

#include <iostream>
#include <string>
#include <cmath>

using namespace std;

// ══════════════════════════════════════════════════════════════════
// 1. 基础类：Student
// ══════════════════════════════════════════════════════════════════
class Student {
private:
    string name;
    int    age;
    double gpa;
    static int total_count;  // static：所有对象共享

public:
    // 构造函数（成员初始化列表方式，效率更高）
    Student(const string& n, int a, double g)
        : name(n), age(a), gpa(g) {
        total_count++;
        cout << "[构造] " << name << " 被创建" << endl;
    }

    // 默认构造函数
    Student() : name("未命名"), age(0), gpa(0.0) {
        total_count++;
        cout << "[构造] 默认学生被创建" << endl;
    }

    // 拷贝构造函数
    Student(const Student& other)
        : name(other.name), age(other.age), gpa(other.gpa) {
        total_count++;
        cout << "[拷贝构造] " << name << " 被复制" << endl;
    }

    // 赋值运算符
    Student& operator=(const Student& other) {
        if (this != &other) {  // 防止自赋值
            name = other.name;
            age  = other.age;
            gpa  = other.gpa;
            cout << "[赋值] " << name << " 被赋值" << endl;
        }
        return *this;  // 支持链式赋值
    }

    // 析构函数
    ~Student() {
        total_count--;
        cout << "[析构] " << name << " 被销毁" << endl;
    }

    // ── Getter（const 成员函数，不修改对象） ──────────────────────
    string getName()  const { return name; }
    int    getAge()   const { return age;  }
    double getGpa()   const { return gpa;  }

    // ── Setter ────────────────────────────────────────────────────
    void setAge(int a) {
        if (a >= 0 && a <= 150) age = a;
    }
    void setGpa(double g) {
        if (g >= 0.0 && g <= 4.0) gpa = g;
    }

    // ── 显示信息 ──────────────────────────────────────────────────
    void display() const {
        cout << "姓名：" << name
             << "  年龄：" << age
             << "  GPA：" << gpa << endl;
    }

    // ── static 函数 ───────────────────────────────────────────────
    static int getCount() { return total_count; }

    // ── 运算符重载：< 用于排序比较 ───────────────────────────────
    bool operator<(const Student& other) const {
        return gpa < other.gpa;  // 按 GPA 比较
    }

    // ── 友元：允许 operator<< 访问私有成员 ───────────────────────
    friend ostream& operator<<(ostream& os, const Student& s);
};

// static 成员变量在类外初始化
int Student::total_count = 0;

// 友元函数：重载输出运算符
ostream& operator<<(ostream& os, const Student& s) {
    os << "Student{" << s.name << ", " << s.age << ", GPA=" << s.gpa << "}";
    return os;
}

// ══════════════════════════════════════════════════════════════════
// 2. 练习类：Complex（复数）
// ══════════════════════════════════════════════════════════════════
class Complex {
private:
    double real;
    double imag;

public:
    Complex(double r = 0, double i = 0) : real(r), imag(i) {}

    // 运算符重载：+
    Complex operator+(const Complex& other) const {
        return Complex(real + other.real, imag + other.imag);
    }

    // 运算符重载：-
    Complex operator-(const Complex& other) const {
        return Complex(real - other.real, imag - other.imag);
    }

    // 运算符重载：* （(a+bi)(c+di) = (ac-bd) + (ad+bc)i）
    Complex operator*(const Complex& other) const {
        return Complex(
            real * other.real - imag * other.imag,
            real * other.imag + imag * other.real
        );
    }

    // 运算符重载：== 
    bool operator==(const Complex& other) const {
        return real == other.real && imag == other.imag;
    }

    // 模（magnitude）
    double magnitude() const {
        return sqrt(real * real + imag * imag);
    }

    // 友元输出
    friend ostream& operator<<(ostream& os, const Complex& c) {
        os << c.real;
        if (c.imag >= 0) os << "+";
        os << c.imag << "i";
        return os;
    }
};

int main() {
    cout << "╔══════════════════════════════╗" << endl;
    cout << "║  Student 类演示              ║" << endl;
    cout << "╚══════════════════════════════╝" << endl;

    {
        // 局部作用域，作用域结束时析构
        Student s1("张三", 20, 3.5);
        Student s2("李四", 22, 3.8);
        Student s3 = s1;  // 调用拷贝构造

        cout << "\n学生信息：" << endl;
        s1.display();
        s2.display();
        cout << s3 << endl;  // 使用重载的 <<

        s3 = s2;  // 调用赋值运算符
        cout << "\n赋值后 s3：" << s3 << endl;

        cout << "\n当前学生总数：" << Student::getCount() << endl;

        cout << "\ns1 < s2（GPA比较）？" << (s1 < s2 ? "是" : "否") << endl;

    }  // s1, s2, s3 在此析构
    cout << "\n作用域结束，学生总数：" << Student::getCount() << endl;

    // ─── 复数类 ───────────────────────────────────────────────────
    cout << "\n╔══════════════════════════════╗" << endl;
    cout << "║  Complex 复数类演示           ║" << endl;
    cout << "╚══════════════════════════════╝" << endl;

    Complex c1(3, 4);   // 3 + 4i
    Complex c2(1, -2);  // 1 - 2i

    cout << "c1 = " << c1 << endl;
    cout << "c2 = " << c2 << endl;
    cout << "c1 + c2 = " << (c1 + c2) << endl;
    cout << "c1 - c2 = " << (c1 - c2) << endl;
    cout << "c1 * c2 = " << (c1 * c2) << endl;
    cout << "|c1| = " << c1.magnitude() << endl;
    cout << "c1 == c2 ? " << (c1 == c2 ? "相等" : "不等") << endl;

    return 0;
}
