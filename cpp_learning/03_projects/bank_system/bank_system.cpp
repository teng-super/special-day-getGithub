/**
 * ╔══════════════════════════════════════════════════════════════╗
 * ║         综合练习项目 2：简易银行账户管理系统                   ║
 * ║                                                              ║
 * ║  涵盖技术：                                                   ║
 * ║  - OOP + 继承 + 多态（不同账户类型）                          ║
 * ║  - 智能指针（unique_ptr / shared_ptr）                        ║
 * ║  - 移动语义                                                   ║
 * ║  - 运算符重载                                                 ║
 * ║  - 模板（通用事务记录）                                       ║
 * ║  - lambda & functional                                        ║
 * ║  - 异常处理                                                   ║
 * ║  - 文件 I/O 持久化                                            ║
 * ║  - C++11 特性：enum class, auto, initializer_list            ║
 * ╚══════════════════════════════════════════════════════════════╝
 *
 * 编译：g++ -std=c++17 -o bank bank_system.cpp
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <memory>
#include <stdexcept>
#include <iomanip>
#include <string>
#include <functional>
#include <algorithm>
#include <ctime>
#include <chrono>

using namespace std;

// ══════════════════════════════════════════════════════════════════
// 工具：当前时间字符串
// ══════════════════════════════════════════════════════════════════
string current_time() {
    auto now = chrono::system_clock::now();
    time_t t  = chrono::system_clock::to_time_t(now);
    char buf[32];
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", localtime(&t));
    return string(buf);
}

// ══════════════════════════════════════════════════════════════════
// 异常类
// ══════════════════════════════════════════════════════════════════
class BankException : public runtime_error {
    int code_;
public:
    BankException(const string& msg, int code = -1)
        : runtime_error(msg), code_(code) {}
    int code() const { return code_; }
};

class InsufficientFundsException : public BankException {
public:
    InsufficientFundsException(double balance, double amount)
        : BankException(
            "余额不足：当前余额 " + to_string(balance) +
            "，需要 " + to_string(amount), 402) {}
};

class AccountNotFoundException : public BankException {
public:
    explicit AccountNotFoundException(const string& id)
        : BankException("账户不存在：" + id, 404) {}
};

class InvalidAmountException : public BankException {
public:
    explicit InvalidAmountException(double amount)
        : BankException("无效金额：" + to_string(amount), 400) {}
};

// ══════════════════════════════════════════════════════════════════
// 事务类型
// ══════════════════════════════════════════════════════════════════
enum class TransactionType {
    DEPOSIT,    // 存款
    WITHDRAW,   // 取款
    TRANSFER,   // 转账
    INTEREST,   // 利息
    FEE         // 手续费
};

string type_name(TransactionType t) {
    switch (t) {
        case TransactionType::DEPOSIT:  return "存款";
        case TransactionType::WITHDRAW: return "取款";
        case TransactionType::TRANSFER: return "转账";
        case TransactionType::INTEREST: return "利息";
        case TransactionType::FEE:      return "手续费";
        default:                        return "未知";
    }
}

// ══════════════════════════════════════════════════════════════════
// 事务记录（模板支持多种金额类型）
// ══════════════════════════════════════════════════════════════════
template <typename AmountT = double>
struct Transaction {
    string          timestamp;
    TransactionType type;
    AmountT         amount;
    AmountT         balance_after;
    string          note;

    Transaction(TransactionType t, AmountT amt, AmountT bal, const string& n = "")
        : timestamp(current_time()), type(t), amount(amt),
          balance_after(bal), note(n) {}

    void print() const {
        cout << "  " << setw(20) << left << timestamp
             << setw(6)  << type_name(type)
             << setw(12) << fixed << setprecision(2) << amount
             << setw(12) << balance_after;
        if (!note.empty()) cout << " (" << note << ")";
        cout << endl;
    }
};

// ══════════════════════════════════════════════════════════════════
// 抽象基类：Account
// ══════════════════════════════════════════════════════════════════
class Account {
protected:
    string   account_id;
    string   owner_name;
    double   balance;
    vector<Transaction<double>> history;

    static void validate_amount(double amount) {
        if (amount <= 0)
            throw InvalidAmountException(amount);
    }

public:
    Account(const string& id, const string& owner, double initial = 0.0)
        : account_id(id), owner_name(owner), balance(initial) {
        if (initial > 0)
            history.emplace_back(TransactionType::DEPOSIT, initial, initial, "初始存款");
    }

    virtual ~Account() = default;

    // ── 基本操作 ──────────────────────────────────────────────────
    virtual void deposit(double amount, const string& note = "") {
        validate_amount(amount);
        balance += amount;
        history.emplace_back(TransactionType::DEPOSIT, amount, balance, note);
        cout << "  ✓ 存款 " << fixed << setprecision(2) << amount
             << " 元，当前余额：" << balance << " 元" << endl;
    }

    virtual void withdraw(double amount, const string& note = "") {
        validate_amount(amount);
        if (amount > balance)
            throw InsufficientFundsException(balance, amount);
        balance -= amount;
        history.emplace_back(TransactionType::WITHDRAW, amount, balance, note);
        cout << "  ✓ 取款 " << fixed << setprecision(2) << amount
             << " 元，当前余额：" << balance << " 元" << endl;
    }

    // 转账：从本账户转出，由 Bank 调用接收方的 deposit
    virtual double transfer_out(double amount) {
        validate_amount(amount);
        if (amount > balance)
            throw InsufficientFundsException(balance, amount);
        balance -= amount;
        history.emplace_back(TransactionType::TRANSFER, amount, balance, "转出");
        return amount;
    }

    void transfer_in(double amount, const string& from_id) {
        balance += amount;
        history.emplace_back(TransactionType::TRANSFER, amount, balance,
                              "来自 " + from_id);
    }

    // 纯虚：不同账户类型的年化计息
    virtual void apply_interest() = 0;
    virtual string account_type() const = 0;

    // ── 显示 ──────────────────────────────────────────────────────
    virtual void display() const {
        cout << "┌─────────────────────────────────────┐" << endl;
        cout << "│ 账户：" << setw(15) << left << account_id
             << " 类型：" << setw(10) << account_type() << "│" << endl;
        cout << "│ 户主：" << setw(15) << owner_name
             << "              │" << endl;
        cout << "│ 余额：" << fixed << setprecision(2) << setw(12) << balance
             << " 元              │" << endl;
        cout << "└─────────────────────────────────────┘" << endl;
    }

    void print_history(int last_n = 10) const {
        cout << "\n  [" << account_id << "] 最近 " << last_n << " 条交易记录：" << endl;
        cout << "  " << setw(20) << left << "时间"
             << setw(6) << "类型" << setw(12) << "金额" << "余额" << endl;
        cout << "  " << string(52, '-') << endl;
        int start = max(0, (int)history.size() - last_n);
        for (int i = start; i < (int)history.size(); i++)
            history[i].print();
    }

    // ── Getter ────────────────────────────────────────────────────
    string getId()      const { return account_id; }
    string getOwner()   const { return owner_name; }
    double getBalance() const { return balance; }
    size_t tx_count()   const { return history.size(); }
};

// ══════════════════════════════════════════════════════════════════
// 储蓄账户：有年利率，无手续费，不可透支
// ══════════════════════════════════════════════════════════════════
class SavingsAccount : public Account {
private:
    double annual_rate;  // 年利率

public:
    SavingsAccount(const string& id, const string& owner,
                   double initial = 0.0, double rate = 0.035)
        : Account(id, owner, initial), annual_rate(rate) {}

    string account_type() const override { return "储蓄账户"; }

    void apply_interest() override {
        double interest = balance * annual_rate / 12;  // 月利息
        balance += interest;
        history.emplace_back(TransactionType::INTEREST, interest, balance,
                              "月利息(" + to_string(annual_rate * 100) + "%)");
        cout << "  [" << account_id << "] 计息 +"
             << fixed << setprecision(2) << interest << " 元" << endl;
    }

    void display() const override {
        Account::display();
        cout << "  年利率：" << fixed << setprecision(2) << annual_rate * 100 << "%" << endl;
    }

    double getRate() const { return annual_rate; }
};

// ══════════════════════════════════════════════════════════════════
// 信用卡账户：有透支额度、月费
// ══════════════════════════════════════════════════════════════════
class CreditAccount : public Account {
private:
    double credit_limit;   // 透支额度
    double monthly_fee;    // 月费
    double interest_rate;  // 透支利率

public:
    CreditAccount(const string& id, const string& owner,
                  double limit = 5000.0, double fee = 0.0,
                  double rate = 0.18)
        : Account(id, owner, 0.0),
          credit_limit(limit), monthly_fee(fee), interest_rate(rate) {}

    string account_type() const override { return "信用卡"; }

    // 可透支取款
    void withdraw(double amount, const string& note = "") override {
        validate_amount(amount);
        if (amount > balance + credit_limit)
            throw BankException(
                "超过信用额度！可用额度：" + to_string(balance + credit_limit), 403);
        balance -= amount;
        history.emplace_back(TransactionType::WITHDRAW, amount, balance, note);
        cout << "  ✓ 取款 " << fixed << setprecision(2) << amount
             << " 元，当前余额：" << balance << " 元";
        if (balance < 0)
            cout << "（欠款 " << -balance << " 元，利率 "
                 << interest_rate * 100 << "%）";
        cout << endl;
    }

    void apply_interest() override {
        // 月费
        if (monthly_fee > 0) {
            balance -= monthly_fee;
            history.emplace_back(TransactionType::FEE, monthly_fee, balance, "月费");
        }
        // 透支利息
        if (balance < 0) {
            double interest = (-balance) * interest_rate / 12;
            balance -= interest;
            history.emplace_back(TransactionType::INTEREST, interest, balance,
                                  "透支利息(" + to_string(interest_rate * 100) + "%)");
            cout << "  [" << account_id << "] 透支利息 -"
                 << fixed << setprecision(2) << interest << " 元" << endl;
        }
    }

    void display() const override {
        Account::display();
        cout << "  信用额度：" << fixed << setprecision(2) << credit_limit
             << " 元  可用：" << balance + credit_limit << " 元" << endl;
    }
};

// ══════════════════════════════════════════════════════════════════
// 定期存款账户：不可随意取款，到期才能取
// ══════════════════════════════════════════════════════════════════
class FixedDepositAccount : public Account {
private:
    double annual_rate;
    int    months;         // 定期月数
    bool   matured;

public:
    FixedDepositAccount(const string& id, const string& owner,
                        double initial, int m, double rate = 0.05)
        : Account(id, owner, initial), annual_rate(rate), months(m), matured(false) {}

    string account_type() const override { return "定期存款"; }

    void withdraw(double amount, const string& note = "") override {
        if (!matured)
            throw BankException("定期存款尚未到期，无法取款！", 403);
        Account::withdraw(amount, note);
    }

    void apply_interest() override {
        double interest = balance * annual_rate / 12;
        balance += interest;
        history.emplace_back(TransactionType::INTEREST, interest, balance,
                              "定期利息(" + to_string(annual_rate * 100) + "%)");
        months--;
        if (months <= 0) {
            matured = true;
            cout << "  [" << account_id << "] 定期存款已到期！"
                 << "余额：" << fixed << setprecision(2) << balance << " 元" << endl;
        }
    }

    void display() const override {
        Account::display();
        cout << "  年利率：" << annual_rate * 100 << "%"
             << "  剩余月数：" << months
             << "  状态：" << (matured ? "已到期" : "定期中") << endl;
    }
};

// ══════════════════════════════════════════════════════════════════
// 银行系统
// ══════════════════════════════════════════════════════════════════
class Bank {
private:
    string name_;
    map<string, unique_ptr<Account>> accounts;
    int next_id = 1000;

    Account& get_account(const string& id) {
        auto it = accounts.find(id);
        if (it == accounts.end())
            throw AccountNotFoundException(id);
        return *it->second;
    }

    const Account& get_account(const string& id) const {
        auto it = accounts.find(id);
        if (it == accounts.end())
            throw AccountNotFoundException(id);
        return *it->second;
    }

    string generate_id(const string& prefix) {
        return prefix + to_string(next_id++);
    }

public:
    explicit Bank(const string& n) : name_(n) {
        cout << "=== " << name_ << " 银行系统启动 ===" << endl;
    }

    // ── 开户 ──────────────────────────────────────────────────────
    string open_savings(const string& owner, double initial = 0.0, double rate = 0.035) {
        string id = generate_id("SAV");
        accounts[id] = make_unique<SavingsAccount>(id, owner, initial, rate);
        cout << "✓ 开户成功：" << owner << " [" << id << "] 储蓄账户"
             << " 初始存款：" << initial << " 元" << endl;
        return id;
    }

    string open_credit(const string& owner, double limit = 5000.0) {
        string id = generate_id("CRD");
        accounts[id] = make_unique<CreditAccount>(id, owner, limit);
        cout << "✓ 开户成功：" << owner << " [" << id << "] 信用卡"
             << " 额度：" << limit << " 元" << endl;
        return id;
    }

    string open_fixed(const string& owner, double amount, int months, double rate = 0.05) {
        string id = generate_id("FIX");
        accounts[id] = make_unique<FixedDepositAccount>(id, owner, amount, months, rate);
        cout << "✓ 开户成功：" << owner << " [" << id << "] 定期存款 "
             << months << "个月 " << amount << " 元" << endl;
        return id;
    }

    // ── 存取款 ────────────────────────────────────────────────────
    void deposit(const string& id, double amount) {
        get_account(id).deposit(amount);
    }

    void withdraw(const string& id, double amount) {
        get_account(id).withdraw(amount);
    }

    // ── 转账 ──────────────────────────────────────────────────────
    void transfer(const string& from_id, const string& to_id, double amount) {
        auto& from = get_account(from_id);
        auto& to   = get_account(to_id);
        double transferred = from.transfer_out(amount);
        to.transfer_in(transferred, from_id);
        cout << "  ✓ 转账：[" << from_id << "] -> [" << to_id << "]"
             << " " << fixed << setprecision(2) << amount << " 元" << endl;
    }

    // ── 月结算（计息）────────────────────────────────────────────
    void month_end_settlement() {
        cout << "\n=== 月末结算 ===" << endl;
        for (auto& [id, acc] : accounts) acc->apply_interest();
    }

    // ── 查询 ──────────────────────────────────────────────────────
    void show_account(const string& id) const {
        get_account(id).display();
    }

    void show_history(const string& id, int n = 10) const {
        get_account(id).print_history(n);
    }

    void list_all() const {
        cout << "\n=== 所有账户 ===" << endl;
        cout << setw(12) << left << "账户号"
             << setw(10) << "户主"
             << setw(12) << "类型"
             << "余额" << endl;
        cout << string(50, '-') << endl;
        for (const auto& [id, acc] : accounts) {
            cout << setw(12) << left << acc->getId()
                 << setw(10) << acc->getOwner()
                 << setw(12) << acc->account_type()
                 << fixed << setprecision(2) << acc->getBalance() << " 元" << endl;
        }
        cout << "\n共 " << accounts.size() << " 个账户" << endl;
    }

    // 总资产
    double total_assets() const {
        double total = 0;
        for (const auto& [id, acc] : accounts)
            if (acc->getBalance() > 0) total += acc->getBalance();
        return total;
    }

    // 按条件筛选账户
    vector<string> find_accounts(function<bool(const Account&)> pred) const {
        vector<string> result;
        for (const auto& [id, acc] : accounts)
            if (pred(*acc)) result.push_back(id);
        return result;
    }
};

// ══════════════════════════════════════════════════════════════════
// main：演示
// ══════════════════════════════════════════════════════════════════
int main() {
    Bank bank("工大");

    // ─── 开户 ─────────────────────────────────────────────────────
    cout << "\n--- 开户 ---" << endl;
    string alice_sav = bank.open_savings("Alice", 10000.0, 0.04);
    string bob_sav   = bank.open_savings("Bob",    5000.0, 0.035);
    string alice_crd = bank.open_credit("Alice", 10000.0);
    string tom_fix   = bank.open_fixed("Tom",  20000.0, 6, 0.05);

    // ─── 存取款 ───────────────────────────────────────────────────
    cout << "\n--- 存取款 ---" << endl;
    bank.deposit(alice_sav, 5000);
    bank.withdraw(alice_sav, 2000);
    bank.deposit(bob_sav, 1000);

    // 信用卡消费（透支）
    bank.withdraw(alice_crd, 3000);
    bank.withdraw(alice_crd, 5000);  // 进入透支

    // ─── 转账 ─────────────────────────────────────────────────────
    cout << "\n--- 转账 ---" << endl;
    bank.transfer(alice_sav, bob_sav, 1500);

    // ─── 月末结算 ─────────────────────────────────────────────────
    bank.month_end_settlement();

    // ─── 查看信息 ─────────────────────────────────────────────────
    cout << "\n--- 账户信息 ---" << endl;
    bank.list_all();

    cout << "\n--- Alice 储蓄卡详情 ---" << endl;
    bank.show_account(alice_sav);
    bank.show_history(alice_sav);

    cout << "\n--- Alice 信用卡详情 ---" << endl;
    bank.show_account(alice_crd);
    bank.show_history(alice_crd);

    cout << "\n--- Tom 定期存款 ---" << endl;
    bank.show_account(tom_fix);

    // 尝试取定期存款（未到期）
    try {
        bank.withdraw(tom_fix, 1000);
    } catch (const BankException& e) {
        cout << "  捕获异常：" << e.what() << " (code=" << e.code() << ")" << endl;
    }

    // ─── Lambda 过滤 ──────────────────────────────────────────────
    cout << "\n--- 余额大于 10000 的账户 ---" << endl;
    auto rich = bank.find_accounts([](const Account& a){ return a.getBalance() > 10000; });
    for (const string& id : rich) bank.show_account(id);

    cout << "\n=== 银行总管理资产：" << fixed << setprecision(2)
         << bank.total_assets() << " 元 ===" << endl;

    return 0;
}
