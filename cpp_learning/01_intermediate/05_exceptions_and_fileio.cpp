/**
 * 【C++ 中级 - 第5课：异常处理 & 文件 I/O】
 *
 * 知识点：
 * 1. 异常（Exception）：throw / try / catch
 * 2. 标准异常类层次（std::exception 家族）
 * 3. 自定义异常类
 * 4. RAII（Resource Acquisition Is Initialization）原则
 * 5. noexcept（C++11）：声明函数不抛出异常
 * 6. 文件读取：ifstream
 * 7. 文件写入：ofstream
 * 8. 二进制文件读写
 * 9. stringstream：内存中的字符串流
 *
 * 练习：
 *   - 读取一个文本文件并统计行数、单词数、字符数
 *   - 将学生信息写入 CSV，再读取回来
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>
#include <filesystem>  // C++17（可降级）

using namespace std;

// ══════════════════════════════════════════════════════════════════
// 1. 自定义异常类
// ══════════════════════════════════════════════════════════════════
class AppException : public runtime_error {
private:
    int error_code;
public:
    AppException(const string& msg, int code = -1)
        : runtime_error(msg), error_code(code) {}

    int getCode() const { return error_code; }
};

class ValidationException : public AppException {
private:
    string field;
public:
    ValidationException(const string& f, const string& msg)
        : AppException("验证失败 [" + f + "]: " + msg, 400), field(f) {}

    string getField() const { return field; }
};

// ══════════════════════════════════════════════════════════════════
// 2. 演示异常层次
// ══════════════════════════════════════════════════════════════════
void process(int code) {
    switch (code) {
        case 1: throw out_of_range("索引越界：-1");
        case 2: throw invalid_argument("参数无效：负数");
        case 3: throw AppException("应用错误：数据库连接失败", 500);
        case 4: throw ValidationException("age", "年龄不能为负数");
        default: cout << "code=" << code << " 正常处理" << endl;
    }
}

// ══════════════════════════════════════════════════════════════════
// 3. noexcept
// ══════════════════════════════════════════════════════════════════
int safe_divide(int a, int b) noexcept {
    if (b == 0) return 0;   // 不抛异常，直接返回默认值
    return a / b;
}

// ══════════════════════════════════════════════════════════════════
// 4. RAII 文件句柄
// ══════════════════════════════════════════════════════════════════
// ifstream/ofstream 本身就是 RAII（析构时自动关闭文件）
// 演示原理：
class FileHandle {
private:
    FILE* fp;
    string filename;
public:
    explicit FileHandle(const string& fname, const char* mode) {
        fp = fopen(fname.c_str(), mode);
        filename = fname;
        if (!fp) throw AppException("无法打开文件：" + fname, 404);
        cout << "[RAII] 打开文件：" << fname << endl;
    }
    ~FileHandle() {
        if (fp) {
            fclose(fp);
            cout << "[RAII] 关闭文件：" << filename << endl;
        }
    }
    FILE* get() { return fp; }

    // 禁止拷贝（防止双重关闭）
    FileHandle(const FileHandle&)            = delete;
    FileHandle& operator=(const FileHandle&) = delete;
};

// ══════════════════════════════════════════════════════════════════
// 5. 学生数据结构（用于 CSV）
// ══════════════════════════════════════════════════════════════════
struct StudentRecord {
    int    id;
    string name;
    double gpa;

    string to_csv() const {
        return to_string(id) + "," + name + "," + to_string(gpa);
    }

    static StudentRecord from_csv(const string& line) {
        istringstream ss(line);
        string token;
        StudentRecord r;
        getline(ss, token, ','); r.id   = stoi(token);
        getline(ss, token, ','); r.name = token;
        getline(ss, token, ','); r.gpa  = stod(token);
        return r;
    }
};

int main() {
    // ─── 异常处理演示 ─────────────────────────────────────────────
    cout << "=== 异常处理 ===" << endl;
    for (int code = 0; code <= 4; code++) {
        try {
            process(code);
        }
        catch (const ValidationException& e) {
            cout << "验证异常：" << e.what()
                 << " 字段=" << e.getField()
                 << " 错误码=" << e.getCode() << endl;
        }
        catch (const AppException& e) {
            cout << "应用异常：" << e.what()
                 << " 错误码=" << e.getCode() << endl;
        }
        catch (const runtime_error& e) {
            cout << "运行时错误：" << e.what() << endl;
        }
        catch (const exception& e) {
            cout << "标准异常：" << e.what() << endl;
        }
        catch (...) {
            cout << "未知异常！" << endl;
        }
    }

    // ─── noexcept ─────────────────────────────────────────────────
    cout << "\n=== noexcept ===" << endl;
    cout << "safe_divide(10, 3) = " << safe_divide(10, 3) << endl;
    cout << "safe_divide(10, 0) = " << safe_divide(10, 0) << " (避免除零)" << endl;

    // ─── stringstream ─────────────────────────────────────────────
    cout << "\n=== stringstream ===" << endl;
    ostringstream oss;
    oss << "C++ " << 11 << " released in " << 2011;
    string result = oss.str();
    cout << "ostringstream 构建：" << result << endl;

    istringstream iss("3.14 42 Hello World");
    double d; int i; string s1, s2;
    iss >> d >> i >> s1 >> s2;
    cout << "istringstream 解析：d=" << d << " i=" << i
         << " s1=" << s1 << " s2=" << s2 << endl;

    // ─── 文件写入 ─────────────────────────────────────────────────
    const string csv_file = "/tmp/students.csv";
    cout << "\n=== 写入 CSV ===" << endl;
    {
        ofstream ofs(csv_file);
        if (!ofs) {
            cerr << "无法创建文件：" << csv_file << endl;
            return 1;
        }
        // 写入表头
        ofs << "id,name,gpa\n";
        vector<StudentRecord> students = {
            {1, "张三", 3.5},
            {2, "李四", 3.8},
            {3, "王五", 3.2},
            {4, "赵六", 3.9},
        };
        for (const auto& s : students) {
            ofs << s.to_csv() << "\n";
            cout << "写入：" << s.to_csv() << endl;
        }
    }  // ofstream 析构，文件自动关闭

    // ─── 文件读取 ─────────────────────────────────────────────────
    cout << "\n=== 读取 CSV ===" << endl;
    {
        ifstream ifs(csv_file);
        if (!ifs) {
            cerr << "无法打开文件：" << csv_file << endl;
            return 1;
        }
        string line;
        getline(ifs, line);  // 跳过表头
        cout << "表头：" << line << endl;

        vector<StudentRecord> loaded;
        while (getline(ifs, line)) {
            if (!line.empty()) {
                loaded.push_back(StudentRecord::from_csv(line));
            }
        }
        cout << "读取到 " << loaded.size() << " 条记录：" << endl;
        for (const auto& s : loaded) {
            cout << "  id=" << s.id
                 << " name=" << s.name
                 << " gpa=" << s.gpa << endl;
        }
    }

    // ─── 文件统计 ─────────────────────────────────────────────────
    cout << "\n=== 文件统计 ===" << endl;
    {
        ifstream ifs(csv_file);
        int lines = 0, words = 0, chars = 0;
        string line;
        while (getline(ifs, line)) {
            lines++;
            chars += line.length() + 1;  // +1 for newline
            istringstream ss(line);
            string token;
            while (ss >> token) words++;
        }
        cout << "行数=" << lines << " 词数=" << words << " 字符数=" << chars << endl;
    }

    // ─── 追加写入 ─────────────────────────────────────────────────
    cout << "\n=== 追加写入 ===" << endl;
    {
        ofstream ofs(csv_file, ios::app);  // ios::app = append mode
        ofs << "5,新同学,3.6\n";
        cout << "已追加新记录" << endl;
    }

    // 验证追加
    {
        ifstream ifs(csv_file);
        string line;
        int cnt = 0;
        while (getline(ifs, line)) cnt++;
        cout << "追加后总行数（含表头）：" << cnt << endl;
    }

    return 0;
}
