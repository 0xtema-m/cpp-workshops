#include <iostream>
#include <vector>
#include <numeric>
#include <fstream>

struct Student {
    size_t group_id;
    std::string name;
    int age;
    double gpa;
};

struct TDumperOptions {
    std::string TotalGpaOutput;
    std::string GroupGpaOutput;
};

struct TDumperArgs {
    const std::vector<Student>& students;
};

class IDumper {
public:
    virtual void ConfigureOutput(TDumperOptions& args) = 0;
    virtual void Dump(TDumperArgs& args) = 0;
    virtual void Finish() {}
    virtual ~IDumper() = default;
};

class TotalGpaDumper : public IDumper {
public:
    void ConfigureOutput(TDumperOptions& args) override {
        total_gpa_output_.open(args.TotalGpaOutput);
    }

    void Dump(TDumperArgs& args) override {
        total_gpa_sum_ = std::accumulate(args.students.begin(), args.students.end(), total_gpa_sum_, [](double sum, const Student& student) {
            return sum + student.gpa;
        });
        total_gpa_count_ = args.students.size();
    }

    void Finish() override {
        total_gpa_output_ << total_gpa_sum_ / total_gpa_count_ << std::endl;
        total_gpa_output_.close();
    }

private:
    std::ofstream total_gpa_output_;
    size_t total_gpa_count_;
    double total_gpa_sum_;
};

class GroupGpaDumper : public IDumper {
public:
    void ConfigureOutput(TDumperOptions& args) override {
        group_gpa_output_.open(args.GroupGpaOutput);
    }

    void Dump(TDumperArgs& args) override {
        double group_gpa_sum = std::accumulate(args.students.begin(), args.students.end(), 0., [](double sum, const Student& student) {
            return sum + student.gpa;
        });
        group_gpa_output_ << group_gpa_sum / args.students.size() << std::endl;
    }

private:
    std::ofstream group_gpa_output_;
};

int main() {
    std::vector<std::shared_ptr<IDumper>> dumpers = {
        std::make_shared<TotalGpaDumper>(),
        std::make_shared<GroupGpaDumper>()
    };

    TDumperOptions options;
    options.TotalGpaOutput = "total_gpa.txt";
    options.GroupGpaOutput = "group_gpa.txt";

    std::for_each(dumpers.begin(), dumpers.end(), [&options](auto& dumper) {
        dumper->ConfigureOutput(options);
    });

    std::vector<Student> students = {
        {1, "John", 20, 3.5},
        {1, "Jane", 21, 3.7},
        {2, "Jim", 22, 3.9}
    };

    std::unordered_map<size_t, std::vector<Student>> students_by_group;
    for (const auto& student : students) {
        students_by_group[student.group_id].push_back(student);
    }

    for (const auto& [group_id, students] : students_by_group) {
        std::for_each(dumpers.begin(), dumpers.end(), [&students](auto& dumper) {
            TDumperArgs args {
                .students = students
            };
            dumper->Dump(args);
        });
    }

    std::for_each(dumpers.begin(), dumpers.end(), [](auto& dumper) {
        dumper->Finish();
    });
}