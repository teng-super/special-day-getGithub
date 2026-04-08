/**
 * 【C++ 中级 - 第2课：继承与多态】
 *
 * 知识点：
 * 1. 继承（Inheritance）：public / protected / private 继承
 * 2. 方法重写（Override）：子类覆盖父类的虚函数
 * 3. 虚函数（virtual）与纯虚函数（= 0）
 * 4. 抽象类（Abstract Class）：含纯虚函数，不能直接实例化
 * 5. 多态（Polymorphism）：父类指针/引用调用子类方法
 * 6. 虚析构函数：防止通过基类指针删除派生类时内存泄漏
 * 7. override 关键字（C++11）：明确表示重写，防止拼写错误
 * 8. final 关键字（C++11）：禁止继续继承或重写
 * 9. dynamic_cast：安全的多态向下转型
 * 10. 多重继承（Multiple Inheritance）与虚继承
 *
 * 练习：
 *   - 设计一个图形层次结构：Shape -> Circle, Rectangle, Triangle
 */

#include <iostream>
#include <vector>
#include <cmath>
#include <memory>   // unique_ptr（C++11）
#include <string>

using namespace std;

// ══════════════════════════════════════════════════════════════════
// 抽象基类：Shape
// ══════════════════════════════════════════════════════════════════
class Shape {
protected:
    string color;

public:
    // 构造函数
    explicit Shape(const string& c = "黑色") : color(c) {}

    // 纯虚函数 -> Shape 是抽象类，不能直接实例化
    virtual double area()      const = 0;
    virtual double perimeter() const = 0;
    virtual string name()      const = 0;

    // 普通虚函数（有默认实现，子类可选择重写）
    virtual void describe() const {
        cout << "[" << name() << "] 颜色=" << color
             << " 面积=" << area()
             << " 周长=" << perimeter() << endl;
    }

    // ⚠️ 必须声明为虚析构函数！
    // 如果不是虚析构函数，通过 Shape* 删除子类对象时只调用基类析构
    // 注意：析构函数中不能调用纯虚函数，这里用固定字符串
    virtual ~Shape() {
        cout << "[析构] Shape 被销毁" << endl;
    }

    // 颜色 getter
    string getColor() const { return color; }
};

// ══════════════════════════════════════════════════════════════════
// 派生类：Circle
// ══════════════════════════════════════════════════════════════════
class Circle : public Shape {
private:
    double radius;
    static constexpr double PI = 3.14159265358979;

public:
    Circle(double r, const string& c = "红色")
        : Shape(c), radius(r) {}

    // override 关键字：显式表示重写父类虚函数（C++11）
    double area()      const override { return PI * radius * radius; }
    double perimeter() const override { return 2 * PI * radius; }
    string name()      const override { return "圆形"; }

    double getRadius() const { return radius; }
};

// ══════════════════════════════════════════════════════════════════
// 派生类：Rectangle
// ══════════════════════════════════════════════════════════════════
class Rectangle : public Shape {
protected:
    double width, height;

public:
    Rectangle(double w, double h, const string& c = "蓝色")
        : Shape(c), width(w), height(h) {}

    double area()      const override { return width * height; }
    double perimeter() const override { return 2 * (width + height); }
    string name()      const override { return "矩形"; }

    double getWidth()  const { return width; }
    double getHeight() const { return height; }
};

// ══════════════════════════════════════════════════════════════════
// 继续派生：Square 继承自 Rectangle
// ══════════════════════════════════════════════════════════════════
class Square : public Rectangle {
public:
    explicit Square(double side, const string& c = "绿色")
        : Rectangle(side, side, c) {}

    string name() const override { return "正方形"; }

    // final 阻止子类再次重写此函数
    double area() const override final { return width * width; }
};

// ══════════════════════════════════════════════════════════════════
// 派生类：Triangle
// ══════════════════════════════════════════════════════════════════
class Triangle : public Shape {
private:
    double a, b, c;  // 三边长

public:
    Triangle(double a, double b, double c, const string& col = "黄色")
        : Shape(col), a(a), b(b), c(c) {}

    double perimeter() const override { return a + b + c; }

    // 海伦公式
    double area() const override {
        double s = perimeter() / 2.0;
        return sqrt(s * (s - a) * (s - b) * (s - c));
    }

    string name() const override { return "三角形"; }
};

// ══════════════════════════════════════════════════════════════════
// 多重继承示例：Flyable & Swimmable -> Duck
// ══════════════════════════════════════════════════════════════════
class Flyable {
public:
    virtual void fly() { cout << "我会飞！" << endl; }
    virtual ~Flyable() = default;
};

class Swimmable {
public:
    virtual void swim() { cout << "我会游泳！" << endl; }
    virtual ~Swimmable() = default;
};

class Duck : public Flyable, public Swimmable {
public:
    void fly()  override { cout << "鸭子飞翔（低低地飞）" << endl; }
    void swim() override { cout << "鸭子游泳" << endl; }
    void quack() { cout << "嘎嘎！" << endl; }
};

// ══════════════════════════════════════════════════════════════════
// main
// ══════════════════════════════════════════════════════════════════
int main() {
    cout << "=== 多态演示：Shape 派生层次 ===" << endl;

    // 用 unique_ptr 管理堆上的对象（C++11 智能指针）
    vector<unique_ptr<Shape>> shapes;
    shapes.push_back(make_unique<Circle>(5.0, "红色"));
    shapes.push_back(make_unique<Rectangle>(4.0, 6.0, "蓝色"));
    shapes.push_back(make_unique<Square>(3.0, "绿色"));
    shapes.push_back(make_unique<Triangle>(3.0, 4.0, 5.0, "黄色"));

    cout << "\n各图形信息：" << endl;
    double total_area = 0;
    for (const auto& s : shapes) {
        s->describe();              // 多态调用
        total_area += s->area();
    }
    cout << "\n所有图形面积之和：" << total_area << endl;

    // ─── dynamic_cast：安全向下转型 ──────────────────────────────
    cout << "\n=== dynamic_cast 演示 ===" << endl;
    Shape* raw = new Circle(7.0);
    Circle* c = dynamic_cast<Circle*>(raw);
    if (c) {
        cout << "转型成功！半径 = " << c->getRadius() << endl;
    }
    Rectangle* r = dynamic_cast<Rectangle*>(raw);
    if (!r) {
        cout << "Circle -> Rectangle 转型失败（正常）" << endl;
    }
    delete raw;

    // ─── 多重继承 ─────────────────────────────────────────────────
    cout << "\n=== 多重继承：Duck ===" << endl;
    Duck duck;
    duck.fly();
    duck.swim();
    duck.quack();

    // 通过接口指针使用
    Flyable*   fb = &duck;
    Swimmable* sb = &duck;
    fb->fly();
    sb->swim();

    cout << "\n（shapes 作用域结束，虚析构函数被调用）" << endl;
    // unique_ptr 自动析构，触发虚析构函数链

    return 0;
}
