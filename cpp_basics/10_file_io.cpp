/**
 * 10_file_io.cpp
 * C++ 入门：文件 I/O
 * 演示文件的读写操作：ofstream、ifstream、fstream、二进制文件
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

int main() {
    const string filename  = "test_text.txt";
    const string binfile   = "test_bin.dat";

    // ---------- 写入文本文件 ----------
    cout << "=== 写入文本文件 ===" << endl;
    {
        ofstream ofs(filename);
        if (!ofs.is_open()) {
            cerr << "无法打开文件: " << filename << endl;
            return 1;
        }
        ofs << "第一行：Hello, C++!" << endl;
        ofs << "第二行：文件 I/O 示例" << endl;
        ofs << "第三行：数字 " << 42 << "，浮点 " << 3.14 << endl;
        cout << "写入完成: " << filename << endl;
    }  // ofs 超出作用域，自动关闭

    // ---------- 追加写入 ----------
    {
        ofstream ofs(filename, ios::app);
        ofs << "第四行：追加内容" << endl;
    }

    // ---------- 读取文本文件（逐行） ----------
    cout << "\n=== 逐行读取 ===" << endl;
    {
        ifstream ifs(filename);
        if (!ifs.is_open()) {
            cerr << "无法打开文件: " << filename << endl;
            return 1;
        }
        string line;
        int lineNum = 1;
        while (getline(ifs, line)) {
            cout << lineNum++ << ": " << line << endl;
        }
    }

    // ---------- 读取到 stringstream 中处理 ----------
    cout << "\n=== stringstream 解析 ===" << endl;
    {
        ifstream ifs(filename);
        ostringstream oss;
        oss << ifs.rdbuf();   // 全量读入
        string content = oss.str();
        cout << "文件总字符数: " << content.size() << endl;
    }

    // ---------- 写入二进制文件 ----------
    cout << "\n=== 二进制文件 ===" << endl;
    {
        vector<double> data = {1.1, 2.2, 3.3, 4.4, 5.5};
        ofstream ofs(binfile, ios::binary);
        size_t sz = data.size();
        ofs.write(reinterpret_cast<const char*>(&sz), sizeof(sz));
        ofs.write(reinterpret_cast<const char*>(data.data()), sz * sizeof(double));
        cout << "写入 " << sz << " 个 double 到 " << binfile << endl;
    }

    // ---------- 读取二进制文件 ----------
    {
        ifstream ifs(binfile, ios::binary);
        size_t sz = 0;
        ifs.read(reinterpret_cast<char*>(&sz), sizeof(sz));
        vector<double> data(sz);
        ifs.read(reinterpret_cast<char*>(data.data()), sz * sizeof(double));
        cout << "读出数据: ";
        for (double v : data) cout << v << " ";
        cout << endl;
    }

    // ---------- 清理临时文件 ----------
    remove(filename.c_str());
    remove(binfile.c_str());
    cout << "\n已删除临时文件" << endl;

    return 0;
}
