#include <iostream>
#include <vector>
#include <numeric>
#include <fstream>
#include <variant>
#include <type_traits>

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


template <typename T>
concept IDumper = requires(T dumper, TDumperOptions& options, TDumperArgs& args) {
    { dumper.ConfigureOutput(options) } -> std::same_as<void>;
    { dumper.Dump(args) } -> std::same_as<void>;
};

class TotalGpaDumper {
public:
    void ConfigureOutput(TDumperOptions& args) {
        total_gpa_output_.open(args.TotalGpaOutput);
    }

    void Dump(TDumperArgs& args) {
        total_gpa_sum_ = std::accumulate(args.students.begin(), args.students.end(), total_gpa_sum_, [](double sum, const Student& student) {
            return sum + student.gpa;
        });
        total_gpa_count_ = args.students.size();
    }

    void Finish() {
        total_gpa_output_ << total_gpa_sum_ / total_gpa_count_ << std::endl;
        total_gpa_output_.close();
    }

private:
    std::ofstream total_gpa_output_;
    size_t total_gpa_count_;
    double total_gpa_sum_;
};

class GroupGpaDumper {
public:
    void ConfigureOutput(TDumperOptions& args) {
        group_gpa_output_.open(args.GroupGpaOutput);
    }

    void Dump(TDumperArgs& args) {
        double group_gpa_sum = std::accumulate(args.students.begin(), args.students.end(), 0., [](double sum, const Student& student) {
            return sum + student.gpa;
        });
        group_gpa_output_ << group_gpa_sum / args.students.size() << std::endl;
    }

private:
    std::ofstream group_gpa_output_;
};

template<typename T, typename... Args>
struct same_as_any {
    static constexpr bool value = (std::is_same_v<T, Args> || ...);
};

template<typename T, typename... Args>
concept same_as_any_of = (std::is_same_v<T, Args> || ...);

template<IDumper... Dumpers>
class DumpersManager {
public:
    template<same_as_any_of<Dumpers...> T>
    void AddDumper(T&& dumper) {
        dumpers_.push_back(dumpers_t(std::forward<T>(dumper)));
    }

    void ConfigureOutput(TDumperOptions& options) {
        for (auto& dumper : dumpers_) {
            std::visit([&options](auto& dumper) { dumper.ConfigureOutput(options); }, dumper);
        }
    }

    void Dump(TDumperArgs& args) {
        for (auto& dumper : dumpers_) {
            std::visit([&args](auto& dumper) { dumper.Dump(args); }, dumper);
        }
    }

    void Finish() {
        for (auto& dumper : dumpers_) {
            std::visit([](auto& dumper) {
                if constexpr (requires { dumper.Finish(); }) {
                    dumper.Finish();
                }
            }, dumper);
        }
    }

private:
    using dumpers_t = std::variant<Dumpers...>;
    std::vector<dumpers_t> dumpers_;
};

int main() {
    DumpersManager<TotalGpaDumper, GroupGpaDumper> dumpers;
    dumpers.AddDumper(TotalGpaDumper());
    dumpers.AddDumper(GroupGpaDumper());

    TDumperOptions options;
    options.TotalGpaOutput = "total_gpa.txt";
    options.GroupGpaOutput = "group_gpa.txt";

    dumpers.ConfigureOutput(options);

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
        TDumperArgs args {
            .students = students
        };
        dumpers.Dump(args);
    }

    dumpers.Finish();
}
