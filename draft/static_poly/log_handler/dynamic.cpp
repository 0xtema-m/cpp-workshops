#include <iostream>
#include <vector>
#include <memory>

class IChain {
public:
    virtual bool Handle(const std::string& command) = 0;
    virtual ~IChain() = default;
};

class InfoHandler : public IChain {
public:
    bool Handle(const std::string& command) override {
        if (command.starts_with("info")) {
            std::cout << "[INFO] " << std::string_view(command).substr(5) << std::endl;
            return true;
        }
        return false;
    }
};

class ErrorHandler : public IChain {
public:
    bool Handle(const std::string& command) override {
        if (command.starts_with("error")) {
            std::cout << "[ERROR] " << std::string_view(command).substr(6) << std::endl;
            return true;
        }
        return false;
    }
};

class DebugHandler : public IChain {
public:
    bool Handle(const std::string& command) override {
        if (command.starts_with("debug")) {
            std::cout << "[DEBUG] " << std::string_view(command).substr(6) << std::endl;
            return true;
        }
        return false;
    }
};

class Chain {
public:
    Chain(std::vector<std::shared_ptr<IChain>> handlers) : handlers_(std::move(handlers)) {}

    bool Handle(const std::string& command) {
        return std::any_of(
            handlers_.begin(),
            handlers_.end(),
            [&command](const auto& handler) { return handler->Handle(command); }
        );
    }

private:
    std::vector<std::shared_ptr<IChain>> handlers_;
};

int main() {
    std::vector<std::shared_ptr<IChain>> handlers = {
        std::make_shared<InfoHandler>(),
        std::make_shared<ErrorHandler>(),
        std::make_shared<DebugHandler>()
    };
    Chain chain(std::move(handlers));


    std::vector<std::string> commands = {
        "info Hello",
        "error Error",
        "debug Debug",
        "unknown Unknown"
    };

    for (const auto& command : commands) {
        chain.Handle(command);
    }
}
