/**
 * ╔══════════════════════════════════════════════════════════════╗
 * ║         综合练习项目 1：学生成绩管理系统                       ║
 * ║                                                              ║
 * ║  涵盖技术：                                                   ║
 * ║  - OOP：类、继承、多态                                        ║
 * ║  - STL：vector, map, algorithm                               ║
 * ║  - 文件 I/O：CSV 持久化                                       ║
 * ║  - 异常处理                                                   ║
 * ║  - C++11：auto, lambda, smart_ptr, range-for                 ║
 * ║  - 统计分析：平均分、最高最低分、分布                           ║
 * ╚══════════════════════════════════════════════════════════════╝
 *
 * 编译：g++ -std=c++17 -o sms student_management.cpp
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <algorithm>
#include <numeric>
#include <memory>
#include <iomanip>
#include <stdexcept>
#include <string>
#include <functional>

using namespace std;

// ══════════════════════════════════════════════════════════════════
// 常量
// ══════════════════════════════════════════════════════════════════
constexpr double PASS_SCORE = 60.0;
const string DATA_FILE = "/tmp/students.csv";

// ══════════════════════════════════════════════════════════════════
// 异常类
// ══════════════════════════════════════════════════════════════════
class StudentNotFoundException : public runtime_error {
public:
    explicit StudentNotFoundException(const string& id)
        : runtime_error("学生 [" + id + "] 不存在") {}
};

class DuplicateStudentException : public runtime_error {
public:
    explicit DuplicateStudentException(const string& id)
        : runtime_error("学号 [" + id + "] 已存在") {}
};

class InvalidScoreException : public invalid_argument {
public:
    explicit InvalidScoreException(double score)
        : invalid_argument("无效分数：" + to_string(score) + "（必须 0-100）") {}
};

// ══════════════════════════════════════════════════════════════════
// 课程成绩
// ══════════════════════════════════════════════════════════════════
struct CourseScore {
    string course;
    double score;
    int    credit;  // 学分

    double weighted() const { return score * credit; }

    string to_csv() const {
        return course + ":" + to_string(score) + ":" + to_string(credit);
    }

    static CourseScore from_str(const string& s) {
        // 格式："课程名:分数:学分"
        CourseScore cs;
        size_t p1 = s.find(':');
        size_t p2 = s.rfind(':');
        cs.course = s.substr(0, p1);
        cs.score  = stod(s.substr(p1 + 1, p2 - p1 - 1));
        cs.credit = stoi(s.substr(p2 + 1));
        return cs;
    }
};

// ══════════════════════════════════════════════════════════════════
// 学生类
// ══════════════════════════════════════════════════════════════════
class Student {
private:
    string              id;
    string              name;
    int                 age;
    string              major;
    vector<CourseScore> courses;

    // 验证分数
    static void validate_score(double score) {
        if (score < 0 || score > 100)
            throw InvalidScoreException(score);
    }

public:
    Student(const string& i, const string& n, int a, const string& m)
        : id(i), name(n), age(a), major(m) {}

    // ── 成绩操作 ──────────────────────────────────────────────────
    void add_course(const string& course, double score, int credit = 3) {
        validate_score(score);
        // 如果已存在，更新分数
        for (auto& cs : courses) {
            if (cs.course == course) {
                cs.score  = score;
                cs.credit = credit;
                return;
            }
        }
        courses.push_back({course, score, credit});
    }

    bool remove_course(const string& course) {
        auto it = find_if(courses.begin(), courses.end(),
                          [&](const CourseScore& cs){ return cs.course == course; });
        if (it == courses.end()) return false;
        courses.erase(it);
        return true;
    }

    // ── 统计 ──────────────────────────────────────────────────────
    double average_score() const {
        if (courses.empty()) return 0;
        double sum = 0;
        for (const auto& cs : courses) sum += cs.score;
        return sum / courses.size();
    }

    double gpa() const {
        // 加权平均绩点（0-4 scale）
        int    total_credit  = 0;
        double weighted_sum  = 0;
        for (const auto& cs : courses) {
            double points;
            if      (cs.score >= 90) points = 4.0;
            else if (cs.score >= 85) points = 3.7;
            else if (cs.score >= 80) points = 3.3;
            else if (cs.score >= 75) points = 3.0;
            else if (cs.score >= 70) points = 2.7;
            else if (cs.score >= 65) points = 2.3;
            else if (cs.score >= 60) points = 2.0;
            else                     points = 0.0;
            weighted_sum += points * cs.credit;
            total_credit += cs.credit;
        }
        return total_credit > 0 ? weighted_sum / total_credit : 0;
    }

    double max_score() const {
        if (courses.empty()) return 0;
        return max_element(courses.begin(), courses.end(),
            [](const CourseScore& a, const CourseScore& b){ return a.score < b.score; }
        )->score;
    }

    double min_score() const {
        if (courses.empty()) return 0;
        return min_element(courses.begin(), courses.end(),
            [](const CourseScore& a, const CourseScore& b){ return a.score < b.score; }
        )->score;
    }

    bool is_passing() const {
        for (const auto& cs : courses)
            if (cs.score < PASS_SCORE) return false;
        return !courses.empty();
    }

    string grade_level() const {
        double avg = average_score();
        if (avg >= 90) return "优秀";
        if (avg >= 80) return "良好";
        if (avg >= 70) return "中等";
        if (avg >= 60) return "及格";
        return "不及格";
    }

    // ── Getter ────────────────────────────────────────────────────
    string getId()    const { return id;      }
    string getName()  const { return name;    }
    int    getAge()   const { return age;     }
    string getMajor() const { return major;   }
    const vector<CourseScore>& getCourses() const { return courses; }

    // ── 序列化 ────────────────────────────────────────────────────
    string to_csv() const {
        ostringstream oss;
        oss << id << "," << name << "," << age << "," << major;
        for (const auto& cs : courses)
            oss << "," << cs.to_csv();
        return oss.str();
    }

    static shared_ptr<Student> from_csv(const string& line) {
        istringstream ss(line);
        string token;

        getline(ss, token, ',');
        string sid = token;
        getline(ss, token, ',');
        string sname = token;
        getline(ss, token, ',');
        int sage = stoi(token);
        getline(ss, token, ',');
        string smajor = token;

        auto s = make_shared<Student>(sid, sname, sage, smajor);
        while (getline(ss, token, ',')) {
            if (!token.empty()) {
                try {
                    auto cs = CourseScore::from_str(token);
                    s->add_course(cs.course, cs.score, cs.credit);
                } catch (...) {}
            }
        }
        return s;
    }

    // ── 显示 ──────────────────────────────────────────────────────
    void display() const {
        cout << "┌────────────────────────────────────┐" << endl;
        cout << "│ 学号：" << setw(10) << left << id
             << "  姓名：" << setw(8) << name      << "│" << endl;
        cout << "│ 年龄：" << setw(3)  << age
             << "  专业：" << setw(15) << major     << "│" << endl;
        if (!courses.empty()) {
            cout << "│ 课程成绩：" << string(26, ' ')   << "│" << endl;
            for (const auto& cs : courses) {
                cout << "│   " << setw(12) << left << cs.course
                     << " " << setw(6) << fixed << setprecision(1) << cs.score
                     << " (" << cs.credit << "学分)"
                     << string(4, ' ') << "│" << endl;
            }
            cout << "│ 平均分：" << setw(6) << fixed << setprecision(2)
                 << average_score()
                 << "  GPA：" << setw(4) << setprecision(2) << gpa()
                 << "  等级：" << grade_level() << string(3, ' ') << "│" << endl;
        }
        cout << "└────────────────────────────────────┘" << endl;
    }
};

// ══════════════════════════════════════════════════════════════════
// 学生管理系统
// ══════════════════════════════════════════════════════════════════
class StudentManagementSystem {
private:
    map<string, shared_ptr<Student>> students;

    shared_ptr<Student> find_student(const string& id) const {
        auto it = students.find(id);
        if (it == students.end())
            throw StudentNotFoundException(id);
        return it->second;
    }

public:
    // ── CRUD ──────────────────────────────────────────────────────
    void add_student(const string& id, const string& name,
                     int age, const string& major) {
        if (students.count(id))
            throw DuplicateStudentException(id);
        students[id] = make_shared<Student>(id, name, age, major);
        cout << "✓ 添加学生：" << name << " (" << id << ")" << endl;
    }

    void remove_student(const string& id) {
        auto s = find_student(id);
        students.erase(id);
        cout << "✓ 删除学生：" << s->getName() << " (" << id << ")" << endl;
    }

    void add_score(const string& id, const string& course,
                   double score, int credit = 3) {
        find_student(id)->add_course(course, score, credit);
        cout << "✓ 录入成绩：" << id << " " << course
             << " = " << score << " (" << credit << "学分)" << endl;
    }

    void show_student(const string& id) const {
        find_student(id)->display();
    }

    // ── 查询 ──────────────────────────────────────────────────────
    void list_all() const {
        if (students.empty()) {
            cout << "（暂无学生数据）" << endl;
            return;
        }
        cout << "\n共 " << students.size() << " 名学生：" << endl;
        cout << setw(10) << left << "学号"
             << setw(10) << "姓名"
             << setw(6)  << "年龄"
             << setw(15) << "专业"
             << setw(8)  << "平均分"
             << setw(6)  << "GPA"
             << "等级" << endl;
        cout << string(65, '-') << endl;
        for (const auto& [id, s] : students) {
            cout << setw(10) << left << s->getId()
                 << setw(10) << s->getName()
                 << setw(6)  << s->getAge()
                 << setw(15) << s->getMajor()
                 << setw(8)  << fixed << setprecision(2) << s->average_score()
                 << setw(6)  << setprecision(2) << s->gpa()
                 << s->grade_level() << endl;
        }
    }

    // 按平均分排序显示
    void list_sorted_by_score(bool ascending = false) const {
        vector<shared_ptr<Student>> sorted;
        for (const auto& [id, s] : students) sorted.push_back(s);

        sort(sorted.begin(), sorted.end(),
             [ascending](const auto& a, const auto& b) {
                 return ascending ? a->average_score() < b->average_score()
                                  : a->average_score() > b->average_score();
             });

        cout << "\n按平均分" << (ascending ? "升序" : "降序") << "排列：" << endl;
        int rank = 1;
        for (const auto& s : sorted) {
            cout << "  第" << setw(2) << rank++ << "名：["
                 << s->getId() << "] " << setw(8) << left << s->getName()
                 << " 平均分=" << fixed << setprecision(2) << s->average_score()
                 << " GPA=" << setprecision(2) << s->gpa() << endl;
        }
    }

    // 按专业统计
    void stats_by_major() const {
        map<string, vector<double>> major_scores;
        for (const auto& [id, s] : students) {
            if (!s->getCourses().empty())
                major_scores[s->getMajor()].push_back(s->average_score());
        }

        cout << "\n按专业统计：" << endl;
        cout << setw(20) << left << "专业"
             << setw(8)  << "人数"
             << setw(10) << "平均分"
             << setw(10) << "最高分"
             << "最低分" << endl;
        cout << string(58, '-') << endl;
        for (const auto& [major, scores] : major_scores) {
            if (scores.empty()) continue;
            double avg = accumulate(scores.begin(), scores.end(), 0.0) / scores.size();
            double mx  = *max_element(scores.begin(), scores.end());
            double mn  = *min_element(scores.begin(), scores.end());
            cout << setw(20) << left << major
                 << setw(8)  << scores.size()
                 << setw(10) << fixed << setprecision(2) << avg
                 << setw(10) << mx
                 << mn << endl;
        }
    }

    // 成绩分布
    void score_distribution() const {
        int dist[5] = {}; // [0,60) [60,70) [70,80) [80,90) [90,100]
        int total = 0;
        for (const auto& [id, s] : students) {
            for (const auto& cs : s->getCourses()) {
                if      (cs.score <  60) dist[0]++;
                else if (cs.score <  70) dist[1]++;
                else if (cs.score <  80) dist[2]++;
                else if (cs.score <  90) dist[3]++;
                else                     dist[4]++;
                total++;
            }
        }
        if (total == 0) return;

        const char* labels[] = {"<60", "60-69", "70-79", "80-89", "90-100"};
        cout << "\n成绩分布（共 " << total << " 门课）：" << endl;
        for (int i = 0; i < 5; i++) {
            int bars = dist[i] * 30 / max(1, total);
            cout << setw(8) << left << labels[i] << " | "
                 << string(bars, '*') << " " << dist[i]
                 << " (" << fixed << setprecision(1)
                 << 100.0 * dist[i] / total << "%)" << endl;
        }
    }

    // 搜索学生（按名字模糊匹配）
    void search_by_name(const string& keyword) const {
        cout << "\n搜索 \"" << keyword << "\" 的结果：" << endl;
        bool found = false;
        for (const auto& [id, s] : students) {
            if (s->getName().find(keyword) != string::npos) {
                s->display();
                found = true;
            }
        }
        if (!found) cout << "  未找到匹配学生" << endl;
    }

    // 不及格学生列表
    void list_failing() const {
        cout << "\n不及格学生：" << endl;
        bool found = false;
        for (const auto& [id, s] : students) {
            if (!s->is_passing() && !s->getCourses().empty()) {
                for (const auto& cs : s->getCourses()) {
                    if (cs.score < PASS_SCORE) {
                        cout << "  [" << id << "] " << s->getName()
                             << " - " << cs.course
                             << " = " << cs.score << endl;
                        found = true;
                    }
                }
            }
        }
        if (!found) cout << "  无不及格学生 🎉" << endl;
    }

    // ── 文件持久化 ────────────────────────────────────────────────
    void save_to_file(const string& filename = DATA_FILE) const {
        ofstream ofs(filename);
        if (!ofs) throw runtime_error("无法写入文件：" + filename);
        ofs << "id,name,age,major,courses...\n";
        for (const auto& [id, s] : students)
            ofs << s->to_csv() << "\n";
        cout << "✓ 已保存 " << students.size() << " 条记录到 " << filename << endl;
    }

    void load_from_file(const string& filename = DATA_FILE) {
        ifstream ifs(filename);
        if (!ifs) {
            cout << "  （文件不存在，跳过加载）" << endl;
            return;
        }
        string line;
        getline(ifs, line);  // 跳过表头
        int count = 0;
        while (getline(ifs, line)) {
            if (!line.empty()) {
                try {
                    auto s = Student::from_csv(line);
                    students[s->getId()] = s;
                    count++;
                } catch (const exception& e) {
                    cerr << "  解析行失败：" << e.what() << endl;
                }
            }
        }
        cout << "✓ 从文件加载 " << count << " 条记录" << endl;
    }

    size_t size() const { return students.size(); }
};

// ══════════════════════════════════════════════════════════════════
// 交互菜单
// ══════════════════════════════════════════════════════════════════
void print_menu() {
    cout << "\n╔══════════════════════════════════╗" << endl;
    cout << "║     学生成绩管理系统 v1.0         ║" << endl;
    cout << "╠══════════════════════════════════╣" << endl;
    cout << "║ 1. 添加学生                       ║" << endl;
    cout << "║ 2. 录入成绩                       ║" << endl;
    cout << "║ 3. 查看学生信息                   ║" << endl;
    cout << "║ 4. 列出所有学生                   ║" << endl;
    cout << "║ 5. 按分数排序                     ║" << endl;
    cout << "║ 6. 按专业统计                     ║" << endl;
    cout << "║ 7. 成绩分布图                     ║" << endl;
    cout << "║ 8. 搜索学生                       ║" << endl;
    cout << "║ 9. 不及格名单                     ║" << endl;
    cout << "║ 10. 删除学生                      ║" << endl;
    cout << "║ 11. 保存数据                      ║" << endl;
    cout << "║ 12. 加载数据                      ║" << endl;
    cout << "║ 0. 退出                           ║" << endl;
    cout << "╚══════════════════════════════════╝" << endl;
    cout << "请选择：";
}

// ══════════════════════════════════════════════════════════════════
// 演示数据初始化
// ══════════════════════════════════════════════════════════════════
void init_demo_data(StudentManagementSystem& sms) {
    cout << "\n--- 初始化演示数据 ---" << endl;

    // 添加学生
    sms.add_student("2021001", "张三",   21, "计算机科学");
    sms.add_student("2021002", "李四",   20, "软件工程");
    sms.add_student("2021003", "王五",   22, "计算机科学");
    sms.add_student("2021004", "赵六",   21, "数据科学");
    sms.add_student("2021005", "孙七",   20, "软件工程");

    // 录入成绩
    sms.add_score("2021001", "高等数学",  85, 4);
    sms.add_score("2021001", "线性代数",  92, 3);
    sms.add_score("2021001", "C++程序设计", 95, 3);
    sms.add_score("2021001", "数据结构",   88, 3);

    sms.add_score("2021002", "高等数学",  78, 4);
    sms.add_score("2021002", "线性代数",  82, 3);
    sms.add_score("2021002", "C++程序设计", 75, 3);
    sms.add_score("2021002", "数据结构",   55, 3);  // 不及格！

    sms.add_score("2021003", "高等数学",  95, 4);
    sms.add_score("2021003", "线性代数",  98, 3);
    sms.add_score("2021003", "C++程序设计", 96, 3);
    sms.add_score("2021003", "数据结构",   93, 3);

    sms.add_score("2021004", "高等数学",  68, 4);
    sms.add_score("2021004", "统计学",    75, 3);
    sms.add_score("2021004", "Python编程", 82, 3);
    sms.add_score("2021004", "机器学习",   58, 3);  // 不及格！

    sms.add_score("2021005", "高等数学",  90, 4);
    sms.add_score("2021005", "线性代数",  88, 3);
    sms.add_score("2021005", "Java编程",  92, 3);
    sms.add_score("2021005", "数据结构",  85, 3);
}

int main() {
    StudentManagementSystem sms;
    init_demo_data(sms);

    // 展示所有功能（非交互模式演示）
    cout << "\n=============================" << endl;
    cout << "         功能演示" << endl;
    cout << "=============================" << endl;

    sms.list_all();
    sms.list_sorted_by_score();
    sms.stats_by_major();
    sms.score_distribution();
    sms.list_failing();
    sms.search_by_name("三");

    cout << "\n--- 查看单个学生 ---" << endl;
    sms.show_student("2021003");

    cout << "\n--- 保存到文件 ---" << endl;
    sms.save_to_file();

    // ── 交互菜单（可选启用） ──────────────────────────────────────
    cout << "\n--- 进入交互模式（输入 0 退出）---" << endl;

    bool running = true;
    while (running) {
        print_menu();
        int choice;
        if (!(cin >> choice)) { cin.clear(); cin.ignore(); continue; }

        try {
            switch (choice) {
                case 1: {
                    string id, name, major; int age;
                    cout << "学号："; cin >> id;
                    cout << "姓名："; cin >> name;
                    cout << "年龄："; cin >> age;
                    cout << "专业："; cin >> major;
                    sms.add_student(id, name, age, major);
                    break;
                }
                case 2: {
                    string id, course; double score; int credit;
                    cout << "学号："; cin >> id;
                    cout << "课程："; cin >> course;
                    cout << "分数："; cin >> score;
                    cout << "学分："; cin >> credit;
                    sms.add_score(id, course, score, credit);
                    break;
                }
                case 3: { string id; cout << "学号："; cin >> id; sms.show_student(id); break; }
                case 4: sms.list_all(); break;
                case 5: sms.list_sorted_by_score(); break;
                case 6: sms.stats_by_major(); break;
                case 7: sms.score_distribution(); break;
                case 8: { string kw; cout << "关键字："; cin >> kw; sms.search_by_name(kw); break; }
                case 9: sms.list_failing(); break;
                case 10: { string id; cout << "学号："; cin >> id; sms.remove_student(id); break; }
                case 11: sms.save_to_file(); break;
                case 12: sms.load_from_file(); break;
                case 0: running = false; cout << "再见！" << endl; break;
                default: cout << "无效选项" << endl;
            }
        } catch (const exception& e) {
            cerr << "错误：" << e.what() << endl;
        }
    }

    return 0;
}
