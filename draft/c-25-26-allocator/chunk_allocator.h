#pragma once

#include <cstddef>

template<typename T, size_t Capacity = 1024, size_t ChunkSize = 1>
class ChunkAllocator {
public:
    using value_type = T;

    template<typename U>
    struct rebind {
        using other = ChunkAllocator<U, Capacity, ChunkSize>;
    };

    ChunkAllocator() {
        buffer_ = new Node[Capacity];
        head_ = buffer_;
        Node* current = head_;
        for (int i = 0; i < Capacity - 1; ++i) {
            current->next = current + 1;
            ++current;
        }
        current->next = nullptr;
    }

    ~ChunkAllocator() {
        delete[] buffer_;
    }

    T* allocate(size_t sz) {
        if (sz > ChunkSize) {
            throw std::bad_alloc{};
        }
        if (head_ == nullptr) {
            throw std::bad_alloc{};
        }
        Node* result = head_;
        head_ = head_->next;

        return reinterpret_cast<T*>(result->buffer);
    }

    void deallocate(T* ptr, size_t) {
        Node* node = reinterpret_cast<Node*>(ptr);
        node->next = head_;
        head_ = node;
    }

    // template<typename U>
    // ChunkAllocator(ChunkAllocator<U, ChunkSize>) {}

private:
    struct alignas(std::max(alignof(T), alignof(void*))) Node {
        union {
            char buffer[sizeof(T) * ChunkSize];
            Node* next;
        };
    };

    Node* buffer_;
    Node* head_;
};
