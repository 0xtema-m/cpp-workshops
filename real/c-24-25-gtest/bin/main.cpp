#include <iostream>
#include <vector>
#include <array>
#include <map>
#include <sstream>
#include <tuple>


int main(int argc, char** argv) {
    // std::vector<int> v(100);
    // for (int element: v) {
    //     std::cout << element << ' ';
    // }

    // v[0] = 2;

    // std::map<int, int> m;
    // m[0]; // creates default element with key = 0
    // m.at(0); //  std::out_of_range()

    // std::stringstream ss("abc qwe");
    // std::string s;
    // ss >> s;

    // ss << "def";

    // std::cout << ss.str();

    std::tuple<int, double, std::string> t{1, 2.5, "qwe"};

    std::cout << std::get<0>(t) << std::endl;
    std::cout << std::get<1>(t) << std::endl;
    std::cout << std::get<2>(t) << std::endl;

    std::cout << std::get<int>(t) << std::endl;
    std::cout << std::get<double>(t) << std::endl;
    std::cout << std::get<std::string>(t) << std::endl;

    const auto& [i, d, s] = t;

    std::cout << i << std::endl;
    std::cout << d << std::endl;
    std::cout << s << std::endl;

    std::tuple<int, int> t2{1, 2};
    std::cout << std::get<0>(t2) << std::endl;

    return 0;
}
