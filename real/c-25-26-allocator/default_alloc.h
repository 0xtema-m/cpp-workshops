#pragma once

/*
Stateless
Stateful
*/

template<typename T>
class Allocator {
public:
    using value_type = T;

    T* allocate(size_t sz) {
        char* data = new char[sizeof(T) * sz];
        return reinterpret_cast<T*>(data);
    }

    void deallocate(T* ptr, [[maybe_unused]] size_t sz) {
        char* data = reinterpret_cast<char*>(ptr);
        delete[] data;
    }
};
