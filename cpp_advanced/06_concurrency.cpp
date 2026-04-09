/**
 * 06_concurrency.cpp
 * C++11 多线程与并发
 * 演示：std::thread、mutex、condition_variable、
 *       atomic、future/promise、async
 */

#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <future>
#include <vector>
#include <queue>
#include <chrono>
#include <numeric>
using namespace std;
using namespace chrono_literals;   // 1s, 100ms 等字面量（C++14）

// ---------- 全局互斥量 ----------
mutex g_mtx;

// ---------- 简单线程函数 ----------
void threadFunc(int id, int count) {
    for (int i = 0; i < count; i++) {
        lock_guard<mutex> lock(g_mtx);   // RAII 自动加/解锁
        cout << "[Thread " << id << "] 计数: " << i << endl;
    }
}

// ---------- atomic 原子计数 ----------
atomic<int> g_counter{0};

void atomicIncrement(int n) {
    for (int i = 0; i < n; i++) g_counter++;
}

// ---------- 生产者-消费者（condition_variable） ----------
mutex              cv_mtx;
condition_variable cv;
queue<int>         g_queue;
bool               g_done = false;

void producer(int n) {
    for (int i = 1; i <= n; i++) {
        this_thread::sleep_for(10ms);
        {
            lock_guard<mutex> lock(cv_mtx);
            g_queue.push(i);
            cout << "[生产者] 生产: " << i << endl;
        }
        cv.notify_one();
    }
    {
        lock_guard<mutex> lock(cv_mtx);
        g_done = true;
    }
    cv.notify_all();
}

void consumer(int id) {
    while (true) {
        unique_lock<mutex> lock(cv_mtx);
        cv.wait(lock, []{ return !g_queue.empty() || g_done; });
        while (!g_queue.empty()) {
            int val = g_queue.front();
            g_queue.pop();
            lock.unlock();
            cout << "[消费者 " << id << "] 消费: " << val << endl;
            lock.lock();
        }
        if (g_done && g_queue.empty()) break;
    }
}

// ---------- 使用 std::async 并行计算 ----------
long long partialSum(const vector<int>& v, size_t from, size_t to) {
    return accumulate(v.begin() + from, v.begin() + to, 0LL);
}

// ======================================================
int main() {
    // ---------- 基本线程 ----------
    cout << "=== 基本线程 ===" << endl;
    thread t1(threadFunc, 1, 3);
    thread t2(threadFunc, 2, 3);
    t1.join();
    t2.join();

    // ---------- lambda 线程 ----------
    cout << "\n=== lambda 线程 ===" << endl;
    vector<thread> threads;
    for (int i = 0; i < 4; i++) {
        threads.emplace_back([i]{
            lock_guard<mutex> lock(g_mtx);
            cout << "Lambda 线程 " << i << "，硬件并发数: "
                 << thread::hardware_concurrency() << endl;
        });
    }
    for (auto& t : threads) t.join();

    // ---------- atomic ----------
    cout << "\n=== atomic 原子操作 ===" << endl;
    vector<thread> ats;
    for (int i = 0; i < 5; i++) ats.emplace_back(atomicIncrement, 1000);
    for (auto& t : ats) t.join();
    cout << "原子计数（应为 5000）: " << g_counter << endl;

    // ---------- 生产者-消费者 ----------
    cout << "\n=== 生产者-消费者 ===" << endl;
    thread prod(producer, 5);
    thread cons1(consumer, 1);
    thread cons2(consumer, 2);
    prod.join(); cons1.join(); cons2.join();

    // ---------- std::async / future ----------
    cout << "\n=== async / future ===" << endl;
    vector<int> bigVec(100000);
    iota(bigVec.begin(), bigVec.end(), 1);
    size_t mid = bigVec.size() / 2;

    auto f1 = async(launch::async, partialSum, ref(bigVec), 0,   mid);
    auto f2 = async(launch::async, partialSum, ref(bigVec), mid, bigVec.size());

    long long total = f1.get() + f2.get();
    cout << "并行求和 [1..100000] = " << total << endl;

    // ---------- promise / future ----------
    cout << "\n=== promise / future ===" << endl;
    promise<string> prom;
    future<string>  fut = prom.get_future();

    thread sender([&prom]{
        this_thread::sleep_for(50ms);
        prom.set_value("来自子线程的消息");
    });

    cout << "等待消息..." << endl;
    cout << "收到: " << fut.get() << endl;
    sender.join();

    return 0;
}
