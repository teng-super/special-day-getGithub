/**
 * 【C++ 高级 C++11 - 第5课：并发编程（多线程）】
 *
 * 知识点：
 * 1. std::thread：创建线程
 * 2. join() vs detach()
 * 3. 数据竞争（Data Race）与未定义行为
 * 4. std::mutex：互斥锁
 * 5. std::lock_guard：RAII 锁
 * 6. std::unique_lock：更灵活的锁
 * 7. std::condition_variable：条件变量（生产者-消费者）
 * 8. std::atomic：无锁原子操作
 * 9. std::async & std::future：异步任务
 * 10. std::promise & std::future：线程间通信
 * 11. 线程池简单实现
 *
 * 编译需要加 -lpthread（Linux）或 /MT（MSVC）
 * g++ -std=c++11 -pthread -o concurrency 05_concurrency.cpp
 */

#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <future>
#include <vector>
#include <queue>
#include <functional>
#include <chrono>
#include <string>
#include <sstream>

using namespace std;
using namespace chrono_literals;  // C++14: 1s, 100ms 等字面量

// ══════════════════════════════════════════════════════════════════
// 辅助：线程安全打印（防止输出交错）
// ══════════════════════════════════════════════════════════════════
mutex cout_mutex;

void safe_print(const string& msg) {
    lock_guard<mutex> lock(cout_mutex);
    cout << msg << endl;
}

// 获取线程 ID 字符串
string tid() {
    ostringstream oss;
    oss << this_thread::get_id();
    return oss.str();
}

// ══════════════════════════════════════════════════════════════════
// 1. 数据竞争演示
// ══════════════════════════════════════════════════════════════════
int unsafe_counter = 0;
atomic<int> safe_counter(0);
mutex counter_mutex;

void increment_unsafe(int times) {
    for (int i = 0; i < times; i++) {
        unsafe_counter++;  // ❌ 非原子操作，数据竞争！
    }
}

void increment_with_mutex(int times) {
    for (int i = 0; i < times; i++) {
        lock_guard<mutex> lock(counter_mutex);
        unsafe_counter++;  // ✅ 加锁保护
    }
}

void increment_atomic(int times) {
    for (int i = 0; i < times; i++) {
        safe_counter++;  // ✅ 原子操作，无锁
    }
}

// ══════════════════════════════════════════════════════════════════
// 2. 生产者-消费者（condition_variable）
// ══════════════════════════════════════════════════════════════════
queue<int>           task_queue;
mutex                queue_mutex;
condition_variable   cv;
bool                 finished = false;

void producer(int id, int count) {
    for (int i = 0; i < count; i++) {
        this_thread::sleep_for(10ms);
        {
            lock_guard<mutex> lock(queue_mutex);
            task_queue.push(id * 100 + i);
            safe_print("  生产者" + to_string(id) + " 生产了任务 " + to_string(id * 100 + i));
        }
        cv.notify_one();  // 通知一个消费者
    }
}

void consumer(int id) {
    while (true) {
        unique_lock<mutex> lock(queue_mutex);
        cv.wait(lock, []{  // 等待，直到条件满足
            return !task_queue.empty() || finished;
        });

        if (task_queue.empty() && finished) break;

        int task = task_queue.front();
        task_queue.pop();
        lock.unlock();  // 处理任务前先解锁

        safe_print("  消费者" + to_string(id) + " [线程" + tid() + "] 处理任务 " + to_string(task));
        this_thread::sleep_for(30ms);  // 模拟处理耗时
    }
    safe_print("  消费者" + to_string(id) + " 退出");
}

// ══════════════════════════════════════════════════════════════════
// 3. 简易线程池
// ══════════════════════════════════════════════════════════════════
class ThreadPool {
private:
    vector<thread>          workers;
    queue<function<void()>> task_q;
    mutex                   mtx;
    condition_variable      cond;
    bool                    stop_flag = false;

public:
    explicit ThreadPool(size_t nthreads) {
        for (size_t i = 0; i < nthreads; i++) {
            workers.emplace_back([this] {
                while (true) {
                    function<void()> task;
                    {
                        unique_lock<mutex> lock(mtx);
                        cond.wait(lock, [this] {
                            return stop_flag || !task_q.empty();
                        });
                        if (stop_flag && task_q.empty()) return;
                        task = move(task_q.front());
                        task_q.pop();
                    }
                    task();
                }
            });
        }
        safe_print("线程池：启动 " + to_string(nthreads) + " 个工作线程");
    }

    // 提交任务，返回 future
    template <typename F, typename... Args>
    auto submit(F&& f, Args&&... args) -> future<decltype(f(args...))> {
        using ReturnType = decltype(f(args...));
        auto task = make_shared<packaged_task<ReturnType()>>(
            bind(forward<F>(f), forward<Args>(args)...)
        );
        future<ReturnType> result = task->get_future();
        {
            lock_guard<mutex> lock(mtx);
            if (stop_flag) throw runtime_error("线程池已停止");
            task_q.push([task]{ (*task)(); });
        }
        cond.notify_one();
        return result;
    }

    ~ThreadPool() {
        {
            lock_guard<mutex> lock(mtx);
            stop_flag = true;
        }
        cond.notify_all();
        for (auto& w : workers) w.join();
        safe_print("线程池：所有工作线程已退出");
    }
};

// ══════════════════════════════════════════════════════════════════
// main
// ══════════════════════════════════════════════════════════════════
int main() {
    // ─── 1. 基本线程创建 ──────────────────────────────────────────
    cout << "=== 基本线程 ===" << endl;
    {
        auto task = [](int id, int ms) {
            this_thread::sleep_for(chrono::milliseconds(ms));
            safe_print("  线程" + to_string(id) + " [" + tid() + "] 完成（延迟" + to_string(ms) + "ms）");
        };

        thread t1(task, 1, 30);
        thread t2(task, 2, 10);
        thread t3(task, 3, 20);

        t1.join();  // 等待线程结束（必须 join 或 detach！）
        t2.join();
        t3.join();
        cout << "  所有线程已结束" << endl;
    }

    // ─── 2. 数据竞争 & 互斥锁 ─────────────────────────────────────
    cout << "\n=== 数据竞争 vs 互斥锁 vs atomic ===" << endl;
    const int TIMES = 100000;

    // 原子计数（推荐）
    safe_counter = 0;
    {
        vector<thread> threads;
        for (int i = 0; i < 4; i++)
            threads.emplace_back(increment_atomic, TIMES);
        for (auto& t : threads) t.join();
    }
    cout << "  atomic 期望=" << 4 * TIMES
         << " 实际=" << safe_counter.load()
         << (safe_counter == 4 * TIMES ? " ✓ 正确" : " ✗ 错误") << endl;

    // ─── 3. async & future ────────────────────────────────────────
    cout << "\n=== std::async & std::future ===" << endl;
    {
        // 异步计算（在另一个线程执行）
        auto compute = [](int n) -> long long {
            long long sum = 0;
            for (int i = 1; i <= n; i++) sum += i;
            safe_print("  compute(" + to_string(n) + ") 完成，sum=" + to_string(sum));
            return sum;
        };

        // launch::async 强制异步；launch::deferred 延迟到 get() 时执行
        auto f1 = async(launch::async, compute, 1000000);
        auto f2 = async(launch::async, compute, 2000000);
        auto f3 = async(launch::async, compute, 3000000);

        safe_print("  任务已提交，主线程继续做其他事...");
        this_thread::sleep_for(5ms);
        safe_print("  主线程等待结果...");

        long long r1 = f1.get();  // 等待结果
        long long r2 = f2.get();
        long long r3 = f3.get();
        cout << "  结果：" << r1 << ", " << r2 << ", " << r3 << endl;
    }

    // ─── 4. promise & future：线程间传值 ─────────────────────────
    cout << "\n=== promise & future ===" << endl;
    {
        promise<string> prom;
        future<string>  fut = prom.get_future();

        thread worker([](promise<string>&& p) {
            this_thread::sleep_for(20ms);
            safe_print("  工作线程：计算完成，设置 promise 值");
            p.set_value("Hello from worker thread!");
        }, move(prom));

        safe_print("  主线程：等待 promise...");
        string result = fut.get();
        safe_print("  主线程：收到 \"" + result + "\"");
        worker.join();
    }

    // ─── 5. 生产者-消费者 ─────────────────────────────────────────
    cout << "\n=== 生产者-消费者（condition_variable）===" << endl;
    {
        finished = false;

        thread c1(consumer, 1);
        thread c2(consumer, 2);

        thread p1(producer, 1, 3);
        thread p2(producer, 2, 3);

        p1.join();
        p2.join();

        this_thread::sleep_for(50ms);
        {
            lock_guard<mutex> lock(queue_mutex);
            finished = true;
        }
        cv.notify_all();

        c1.join();
        c2.join();
    }

    // ─── 6. 线程池 ────────────────────────────────────────────────
    cout << "\n=== 线程池 ===" << endl;
    {
        ThreadPool pool(3);

        vector<future<int>> results;
        for (int i = 0; i < 8; i++) {
            results.push_back(pool.submit([i]() -> int {
                this_thread::sleep_for(chrono::milliseconds(10 * (i % 3 + 1)));
                safe_print("  任务" + to_string(i) + " 执行，结果=" + to_string(i * i));
                return i * i;
            }));
        }

        cout << "  收集结果：";
        for (auto& f : results) cout << f.get() << " ";
        cout << endl;
    }  // ThreadPool 析构，等待所有工作线程结束

    return 0;
}
