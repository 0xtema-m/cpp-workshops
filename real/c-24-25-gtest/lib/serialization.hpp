#pragma once

#include <cinttypes>
#include <vector>
#include <istream>
#include <ostream>
#include <sstream>

namespace ohtuzh {

    enum state {
        kOk, kBad
    };

    class data {
    public:
        data() = default;

        data(
            const std::string& name,
            const std::vector<int32_t> numbers,
            state state)
        : name_(name)
        , numbers_(numbers)
        , state_(state) {}

        bool operator==(const data& other) const {
            return name_ == other.name_ && numbers_ == other.numbers_ && state_ == other.state_;
        }

        void serialize(std::ostream& os) const {
            // "name_|1,1,12,3,|OK\n"
            os << name_ << '|';
            for (int32_t i: numbers_) {
                os << i << ',';
            }
            os << '|';
            if (state_ == state::kOk) {
                os << "OK";
            } else {
                os << "BAD";
            }
            os << '\n';
        }

        void deserialize(std::istream& is) {
            std::string line;
            if (std::getline(is, line)) {
                std::istringstream lineStream(line);
                std::string name;
                std::string numbersStr;
                std::string stateStr;

                // Parse name
                if (std::getline(lineStream, name, '|')) {
                    name_ = name;
                }

                // Parse numbers
                if (std::getline(lineStream, numbersStr, '|')) {
                    numbers_.clear();
                    std::istringstream numbersStream(numbersStr);
                    std::string number;
                    while (std::getline(numbersStream, number, ',')) {
                        if (!number.empty()) { // Ignore empty strings possibly at the end
                            numbers_.push_back(std::stoi(number));
                        }
                    }
                }

                // Parse state
                if (std::getline(lineStream, stateStr)) {
                    if (stateStr == "OK") {
                        state_ = state::kOk;
                    } else if (stateStr == "BAD") {
                        state_ = state::kBad;
                    }
                }
            }
        }

    private:
        std::string name_;
        std::vector<int32_t> numbers_;
        state state_;
    };

} // namespace ohtuzh
