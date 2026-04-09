/**
 * 09_design_patterns.cpp
 * C++ 常用设计模式简练示例
 * 演示：单例、工厂、观察者、策略、RAII
 */

#include <iostream>
#include <memory>
#include <vector>
#include <functional>
#include <string>
#include <map>
#include <algorithm>
using namespace std;

// ============================================================
// 一、单例模式（Singleton）—— 线程安全（C++11 局部静态变量）
// ============================================================
class Logger {
public:
    static Logger& instance() {
        static Logger inst;   // C++11 保证局部静态变量初始化线程安全
        return inst;
    }
    void log(const string& msg) {
        cout << "[LOG] " << msg << endl;
    }
private:
    Logger()  { cout << "Logger 创建" << endl; }
    ~Logger() { cout << "Logger 销毁" << endl; }
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;
};

// ============================================================
// 二、工厂模式（Factory Method）
// ============================================================
struct Shape {
    virtual void draw() const = 0;
    virtual ~Shape() = default;
};

struct Circle    : Shape { void draw() const override { cout << "● 圆形" << endl; } };
struct Square    : Shape { void draw() const override { cout << "■ 正方形" << endl; } };
struct Triangle  : Shape { void draw() const override { cout << "▲ 三角形" << endl; } };

unique_ptr<Shape> createShape(const string& type) {
    if (type == "circle")   return make_unique<Circle>();
    if (type == "square")   return make_unique<Square>();
    if (type == "triangle") return make_unique<Triangle>();
    return nullptr;
}

// ============================================================
// 三、观察者模式（Observer）
// ============================================================
class EventBus {
    map<string, vector<function<void(const string&)>>> handlers;
public:
    void subscribe(const string& event, function<void(const string&)> cb) {
        handlers[event].push_back(move(cb));
    }
    void publish(const string& event, const string& data) {
        if (auto it = handlers.find(event); it != handlers.end()) {
            for (auto& cb : it->second) cb(data);
        }
    }
};

// ============================================================
// 四、策略模式（Strategy）
// ============================================================
class Sorter {
    function<bool(int,int)> cmp;
public:
    explicit Sorter(function<bool(int,int)> c) : cmp(move(c)) {}
    void sort(vector<int>& v) {
        std::sort(v.begin(), v.end(), cmp);
    }
};

// ============================================================
// 五、RAII（资源获取即初始化）
// ============================================================
class FileHandle {
    FILE* fp;
public:
    explicit FileHandle(const char* path, const char* mode)
        : fp(fopen(path, mode)) {
        if (!fp) throw runtime_error("无法打开文件");
        cout << "[RAII] 文件已打开" << endl;
    }
    ~FileHandle() {
        if (fp) { fclose(fp); cout << "[RAII] 文件已关闭" << endl; }
    }
    FILE* get() const { return fp; }
    // 禁止拷贝
    FileHandle(const FileHandle&) = delete;
    FileHandle& operator=(const FileHandle&) = delete;
};

// ============================================================
int main() {
    // 单例
    cout << "=== 单例 ===" << endl;
    Logger::instance().log("程序启动");
    Logger::instance().log("单例保证只有一个实例");

    // 工厂
    cout << "\n=== 工厂 ===" << endl;
    for (const string& type : vector<string>{"circle","square","triangle","unknown"}) {
        auto shape = createShape(type);
        if (shape) shape->draw();
        else cout << "未知类型: " << type << endl;
    }

    // 观察者
    cout << "\n=== 观察者 ===" << endl;
    EventBus bus;
    bus.subscribe("click", [](const string& d){ cout << "处理器A 处理 click: " << d << endl; });
    bus.subscribe("click", [](const string& d){ cout << "处理器B 处理 click: " << d << endl; });
    bus.subscribe("hover", [](const string& d){ cout << "处理器C 处理 hover: " << d << endl; });
    bus.publish("click", "button1");
    bus.publish("hover", "menu");

    // 策略
    cout << "\n=== 策略 ===" << endl;
    vector<int> data = {5, 3, 8, 1, 9, 2};
    Sorter ascSorter([](int a, int b){ return a < b; });
    Sorter descSorter([](int a, int b){ return a > b; });
    ascSorter.sort(data);
    for (int v : data) cout << v << " ";
    cout << endl;
    descSorter.sort(data);
    for (int v : data) cout << v << " ";
    cout << endl;

    // RAII
    cout << "\n=== RAII ===" << endl;
    try {
        FileHandle fh("/tmp/raii_test.txt", "w");
        fputs("RAII 测试\n", fh.get());
        cout << "写入成功" << endl;
    }  // fh 超出作用域，析构函数自动关闭文件
    catch (const exception& e) {
        cerr << e.what() << endl;
    }

    return 0;
}
