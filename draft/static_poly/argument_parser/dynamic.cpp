#include <iostream>
#include <string>
#include <charconv>
#include <vector>
#include <memory>

class IArgument {
public:
    virtual std::string_view Name() const = 0;

    virtual double AsNumber() const { throw std::runtime_error("Argument cannot be converted to number"); }
    virtual std::string_view AsString() const { throw std::runtime_error("Argument cannot be converted to string"); }
    virtual bool AsBool() const { throw std::runtime_error("Argument cannot be converted to bool"); }

    virtual ~IArgument() = default;
};

class NumberArgument : public IArgument {
public:
    NumberArgument(std::string_view name, double value) : name_(name), value_(value) {}

    std::string_view Name() const override { return name_; }
    double AsNumber() const override { return value_; }

private:
    std::string_view name_;
    double value_;
};

class BoolArgument : public IArgument {
public:
    BoolArgument(std::string_view name, bool value) : name_(name), value_(value) {}

    std::string_view Name() const override { return name_; }
    bool AsBool() const override { return value_; }

private:
    std::string_view name_;
    bool value_;
};

class StringArgument : public IArgument {
public:
    StringArgument(std::string_view name, std::string_view value) : name_(name), value_(value) {}

    std::string_view Name() const override { return name_; }
    std::string_view AsString() const override { return value_; }

private:
    std::string_view name_;
    std::string_view value_;
};

class IArgumentParser {
public:
    virtual std::shared_ptr<IArgument> Parse(std::string_view command) = 0;
    virtual ~IArgumentParser() = default;
};

class NumberParser : public IArgumentParser {
public:
    std::shared_ptr<IArgument> Parse(std::string_view command) override {
        const auto it = name_.find("=");
        if (it == std::string_view::npos) {
            return nullptr;
        }

        const auto value = command.substr(it + 1);
        auto [_, ec] = std::from_chars(value.data(), value.data() + value.size(), value_);
        if (ec == std::errc()) {
            return nullptr;
        }

        name_ = command.substr(0, it);
        return std::make_shared<NumberArgument>(name_, value_);
    }

private:
    std::string_view name_;
    double value_;
};

class StringParser : public IArgumentParser {
public:
    std::shared_ptr<IArgument> Parse(std::string_view command) override {
        auto it = command.find("=");
        if (it == std::string_view::npos) {
            return nullptr;
        }
        return std::make_shared<StringArgument>(command.substr(0, it), command.substr(it + 1));
    }
};

class BoolParser : public IArgumentParser {
public:
    std::shared_ptr<IArgument> Parse(std::string_view command) override {
        auto it = command.find("=");
        if (it == std::string_view::npos) {
            return nullptr;
        }
        const auto value = command.substr(it + 1);
        if (value == "true") {
            return std::make_shared<BoolArgument>(command.substr(0, it), true);
        }
        if (value == "false") {
            return std::make_shared<BoolArgument>(command.substr(0, it), false);
        }
        return nullptr;
    }
};

class ArgParser {
public:
    ArgParser(std::vector<std::shared_ptr<IArgumentParser>> parsers) : parsers_(std::move(parsers)) {}

    void Parse(char** argv, int argc) {
        arguments_.clear();
        for (int i = 1; i < argc; ++i) {
            for (const auto& parser : parsers_) {
                auto argument = parser->Parse(argv[i]);
                if (argument) {
                    arguments_.push_back(std::move(argument));
                    break;
                }
            }
        }
    }

    std::shared_ptr<IArgument> GetArgument(std::string_view name) {
        for (const auto& argument : arguments_) {
            if (argument->Name() == name) {
                return argument;
            }
        }
        return nullptr;
    }

    
private:
    std::vector<std::shared_ptr<IArgumentParser>> parsers_;
    std::vector<std::shared_ptr<IArgument>> arguments_;
};

int main(int argc, char** argv) {
    auto numberParser = std::make_shared<NumberParser>();
    auto boolParser = std::make_shared<BoolParser>();
    auto stringParser = std::make_shared<StringParser>();

    ArgParser argParser({numberParser, stringParser, boolParser});
    argParser.Parse(argv, argc);

    std::cout << "Number: " << argParser.GetArgument("--number")->AsNumber() << std::endl;
    std::cout << "String: " << argParser.GetArgument("--string")->AsString() << std::endl;
    std::cout << "Bool: " << argParser.GetArgument("--bool")->AsBool() << std::endl;
}
