#pragma once

#include <cstddef>

template<typename T>
class Allocator {
public:
    using value_type = T;

    T* allocate(size_t sz) {
        char* data = new char[sz * sizeof(T)];
        return reinterpret_cast<T*>(data);
    }

    void deallocate(T* ptr, size_t) {
        delete[] reinterpret_cast<char*>(ptr);
    }
};
