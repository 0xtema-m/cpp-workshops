#pragma once

#include <cstddef>
#include <memory>

template<typename T, size_t Capacity = 64>
class StackAllocator {
public:
    using value_type = T;

    template<typename U>
    struct rebind {
        using other = StackAllocator<U, Capacity>;
    };

    StackAllocator() = default;

    template<typename U>
    StackAllocator(StackAllocator<U, Capacity> other) {}

    T* allocate(size_t sz) {
        if (current_ + sz >= buffer_ + Capacity) {
            throw std::bad_alloc();
        }
        T* result = current_;
        current_ += sz;
        return result;
    }

    void deallocate(T* ptr, size_t sz) {}

private:
    T buffer_[Capacity];
    T* current_ = buffer_;
};
