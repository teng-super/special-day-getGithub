/**
 * 07_oop_basics.cpp
 * C++ 入门：面向对象编程（OOP）基础
 * 演示类的定义、封装、构造/析构函数、访问修饰符、静态成员
 */

#include <iostream>
#include <string>
using namespace std;

// ---------- 类的定义 ----------
class Animal {
private:
    string name;   // 私有成员：只能在类内部访问
    int    age;

protected:
    string species; // 受保护成员：类内部及派生类可访问

public:
    // ---------- 构造函数 ----------
    Animal(const string& name, int age, const string& species)
        : name(name), age(age), species(species) {
        cout << "[Animal] 构造: " << name << endl;
    }

    // ---------- 拷贝构造函数 ----------
    Animal(const Animal& other)
        : name(other.name + "_copy"), age(other.age), species(other.species) {
        cout << "[Animal] 拷贝构造: " << name << endl;
    }

    // ---------- 析构函数 ----------
    ~Animal() {
        cout << "[Animal] 析构: " << name << endl;
    }

    // ---------- Getter / Setter ----------
    string getName()    const { return name; }
    int    getAge()     const { return age; }
    void   setAge(int a)      { if (a >= 0) age = a; }

    // ---------- 普通成员函数 ----------
    void speak() const {
        cout << name << " 说话了（默认）" << endl;
    }

    // ---------- 虚函数（为多态做准备） ----------
    virtual void describe() const {
        cout << "动物: " << name
             << ", 年龄: " << age
             << ", 种类: " << species << endl;
    }

    // ---------- 静态成员函数 ----------
    static void info() {
        cout << "[Animal] 这是所有动物的基类" << endl;
    }
};

// ---------- 继承（公有继承） ----------
class Dog : public Animal {
private:
    string breed;  // 犬种

public:
    Dog(const string& name, int age, const string& breed)
        : Animal(name, age, "犬科"),  // 调用父类构造
          breed(breed) {
        cout << "[Dog] 构造: " << name << endl;
    }

    ~Dog() {
        cout << "[Dog] 析构: " << getName() << endl;
    }

    // 重写父类的 speak
    void speak() const {
        cout << getName() << " 汪汪汪！" << endl;
    }

    // 重写虚函数
    void describe() const override {
        Animal::describe();   // 调用父类版本
        cout << "  犬种: " << breed << endl;
    }
};

// ---------- 静态成员变量 ----------
class Counter {
private:
    static int count;  // 所有实例共享
public:
    Counter()  { count++; cout << "创建第 " << count << " 个对象" << endl; }
    ~Counter() { count--; }
    static int getCount() { return count; }
};
int Counter::count = 0;  // 类外初始化静态成员

// ======================================================
int main() {
    cout << "=== 创建对象 ===" << endl;
    Animal cat("小猫", 3, "猫科");
    cat.describe();
    cat.speak();
    cat.setAge(4);
    cout << "修改年龄后: " << cat.getAge() << endl;

    cout << "\n=== 静态方法 ===" << endl;
    Animal::info();

    cout << "\n=== 继承与多态 ===" << endl;
    Dog dog("旺财", 2, "中华田园犬");
    dog.describe();
    dog.speak();

    cout << "\n=== 多态（基类指针指向派生类） ===" << endl;
    Animal* ptr = &dog;
    ptr->describe();  // 调用 Dog::describe（虚函数机制）

    cout << "\n=== 拷贝构造 ===" << endl;
    Animal cat2(cat);
    cat2.describe();

    cout << "\n=== 静态成员 ===" << endl;
    {
        Counter c1, c2, c3;
        cout << "当前对象数: " << Counter::getCount() << endl;
    }  // c1,c2,c3 析构
    cout << "析构后对象数: " << Counter::getCount() << endl;

    cout << "\n=== 程序结束，自动析构 ===" << endl;
    return 0;
}
