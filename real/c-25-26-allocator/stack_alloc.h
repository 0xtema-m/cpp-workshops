#pragma once

#include <cstddef>

template<typename T, size_t Capacity = 1024>
class StackAlloc {
public:
    using value_type = T;

    template<typename U>
    struct rebind {
        using other = StackAlloc<U, Capacity>;
    };

    StackAlloc() = default;

    template<typename U>
    StackAlloc(StackAlloc<U, Capacity> other) {}

    T* allocate(size_t sz) {
        if (current_ + sz >= buffer_ + Capacity) {
            throw std::bad_alloc{};
        }
        T* result = current_;
        current_ += sz;
        return result;
    }

    void deallocate(T*, size_t) {}

private:
    T buffer_[Capacity];
    T* current_ = buffer_;
};
