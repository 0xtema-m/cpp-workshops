#include <iostream>

template<typename T>
concept IHandler = requires(T t, const std::string& command) {
    { t.Handle(command) } -> std::same_as<bool>;
};

class InfoHandler {
public:
    bool Handle(const std::string& command) {
        if (command.starts_with("info")) {
            std::cout << "[INFO] " << std::string_view(command).substr(5) << std::endl;
            return true;
        }
        return false;
    }
};

class ErrorHandler {
public:
    bool Handle(const std::string& command) {
        if (command.starts_with("error")) {
            std::cout << "[ERROR] " << std::string_view(command).substr(6) << std::endl;
            return true;
        }
        return false;
    }
};

class DebugHandler {
public:
    bool Handle(const std::string& command) {
        if (command.starts_with("debug")) {
            std::cout << "[DEBUG] " << std::string_view(command).substr(6) << std::endl;
            return true;
        }
        return false;
    }
};

template<IHandler... Handlers>
class Chain {
public:
    Chain(Handlers... handlers) : handlers_(std::move(handlers)...) {}

    bool Handle(const std::string& command) {
        return std::apply(
            [&command](auto&&... args) {
                return (args.Handle(command) || ...);
            },
            handlers_
        );
    }

private:
    std::tuple<Handlers...> handlers_;
};

int main() {
    Chain chain(InfoHandler{}, ErrorHandler{}, DebugHandler{});
    chain.Handle("info Hello");
    chain.Handle("error Error");
    chain.Handle("debug Debug");
}