/**
 * 03_smart_pointers.cpp
 * C++11 智能指针
 * 演示：unique_ptr、shared_ptr、weak_ptr 及循环引用问题的解决
 */

#include <iostream>
#include <memory>
#include <string>
#include <vector>
using namespace std;

// ---------- 测试类 ----------
class Resource {
    string name;
public:
    explicit Resource(const string& n) : name(n) {
        cout << "[Resource] 构造: " << name << endl;
    }
    ~Resource() {
        cout << "[Resource] 析构: " << name << endl;
    }
    void use() const { cout << "使用资源: " << name << endl; }
    const string& getName() const { return name; }
};

// ---------- 演示 unique_ptr ----------
void demoUniquePtr() {
    cout << "\n=== unique_ptr ===" << endl;

    // 创建（推荐使用 make_unique，C++14）
    auto up1 = make_unique<Resource>("UniqueRes-1");
    up1->use();

    // 所有权转移（move）
    auto up2 = move(up1);
    cout << "up1 是否为空: " << boolalpha << (up1 == nullptr) << endl;
    up2->use();

    // 自定义删除器
    auto deleter = [](Resource* r) {
        cout << "[自定义删除器] 释放 " << r->getName() << endl;
        delete r;
    };
    unique_ptr<Resource, decltype(deleter)> up3(new Resource("UniqueRes-2"), deleter);
    up3->use();

    // unique_ptr 管理数组
    auto arr = make_unique<int[]>(5);
    for (int i = 0; i < 5; i++) arr[i] = (i + 1) * 10;
    cout << "数组: ";
    for (int i = 0; i < 5; i++) cout << arr[i] << " ";
    cout << endl;
}  // up2, up3, arr 超出作用域自动释放

// ---------- 演示 shared_ptr ----------
void demoSharedPtr() {
    cout << "\n=== shared_ptr ===" << endl;

    auto sp1 = make_shared<Resource>("SharedRes");
    cout << "引用计数: " << sp1.use_count() << endl;  // 1

    {
        auto sp2 = sp1;  // 共享所有权
        cout << "sp2 拷贝后引用计数: " << sp1.use_count() << endl;  // 2
        sp2->use();
    }  // sp2 析构
    cout << "sp2 离开作用域后引用计数: " << sp1.use_count() << endl;  // 1

    // shared_ptr 放入容器
    vector<shared_ptr<Resource>> vec;
    vec.push_back(sp1);
    vec.push_back(make_shared<Resource>("SharedRes-2"));
    cout << "容器中 sp1 引用计数: " << sp1.use_count() << endl;  // 2
}

// ---------- 循环引用演示及 weak_ptr 解决方案 ----------
struct Node {
    int   val;
    // 使用 weak_ptr 打破循环引用
    weak_ptr<Node>   next;   // 若用 shared_ptr 则产生循环引用内存泄漏
    shared_ptr<Node> prev;   // prev 用 shared_ptr 保持强引用
    explicit Node(int v) : val(v) { cout << "[Node] 构造: " << v << endl; }
    ~Node() { cout << "[Node] 析构: " << val << endl; }
};

void demoWeakPtr() {
    cout << "\n=== weak_ptr（避免循环引用） ===" << endl;

    auto n1 = make_shared<Node>(1);
    auto n2 = make_shared<Node>(2);

    n1->next = n2;   // weak_ptr，不增加引用计数
    n2->prev = n1;   // shared_ptr，n1 引用计数 +1

    cout << "n1 引用计数: " << n1.use_count() << endl;  // 2（n2->prev 持有）
    cout << "n2 引用计数: " << n2.use_count() << endl;  // 1（n1->next 是 weak）

    // 通过 weak_ptr 访问（先 lock 转为 shared_ptr）
    if (auto sp = n1->next.lock()) {
        cout << "n1->next->val = " << sp->val << endl;
    }
}  // n1, n2 正常析构，无内存泄漏

// ---------- enable_shared_from_this ----------
class Self : public enable_shared_from_this<Self> {
public:
    shared_ptr<Self> getShared() { return shared_from_this(); }
    void show() { cout << "Self 引用计数: " << shared_from_this().use_count() << endl; }
};

void demoSharedFromThis() {
    cout << "\n=== enable_shared_from_this ===" << endl;
    auto sp = make_shared<Self>();
    cout << "外部计数: " << sp.use_count() << endl;  // 1
    auto sp2 = sp->getShared();
    cout << "getShared 后计数: " << sp.use_count() << endl;  // 2
    sp->show();
}

// ======================================================
int main() {
    demoUniquePtr();
    demoSharedPtr();
    demoWeakPtr();
    demoSharedFromThis();
    cout << "\nmain 结束，所有资源已释放" << endl;
    return 0;
}
