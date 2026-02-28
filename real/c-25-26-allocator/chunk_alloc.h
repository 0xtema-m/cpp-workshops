#pragma once

#include <vector>

// [32] -> [32] -> [32] -> [32] -> [32]


template<typename T, size_t ChunkSize = 64>
class ChunkAlloc {
public:
    using value_type = T;

    ChunkAlloc() {
        head_ = nullptr;
    }

    template<typename U>
    struct rebind {
        using other = ChunkAlloc<U, ChunkSize>;
    };

    template<typename U>
    ChunkAlloc(ChunkAlloc<U, ChunkSize> other) {}

    ~ChunkAlloc() {
        while (head_ != nullptr) {
            Node* next = head_->next_;
            delete head_;
            head_ = next;
        }
    }

    T* allocate(size_t sz) {
        if (sz > ChunkSize) {
            throw std::bad_alloc{};
        }
        if (head_ == nullptr) {
            head_ = new Node();
            head_->next_ = nullptr;
        }
        Node* data = head_;
        head_ = head_->next_;

        return reinterpret_cast<T*>(data->buffer_);
    }

    void deallocate(T* ptr, size_t sz) {
        Node* node = reinterpret_cast<Node*>(ptr);
        node->next_ = head_;
        head_ = node;
    }

private:
    struct alignas(std::max(alignof(T), alignof(void*))) Node {
        union {
            char buffer_[sizeof(T) * ChunkSize];
            Node* next_;
        };
    };
    Node* head_;
};



template<typename T, size_t Capacity = 64, size_t ChunkSize = 64>
class ChunkPreallocatedAlloc {
public:
    using value_type = T;

    ChunkPreallocatedAlloc() {
        buffer_ = new Node[Capacity];
        head_ = buffer_;
        Node* cur = head_;
        for (int i = 0; i < Capacity - 1; ++i) {
            cur->next_ = cur + 1;
            ++cur;
        }
        cur->next_ = nullptr;
    }

    template<typename U>
    struct rebind {
        using other = ChunkPreallocatedAlloc<U, Capacity, ChunkSize>;
    };

    template<typename U>
    ChunkPreallocatedAlloc(ChunkPreallocatedAlloc<U, Capacity, ChunkSize> other) {}

    ~ChunkPreallocatedAlloc() {
        delete[] buffer_;
    }

    T* allocate(size_t sz) {
        if (sz > ChunkSize) {
            throw std::bad_alloc{};
        }
        if (head_ == nullptr) {
            throw std::bad_alloc{};
        }
        Node* data = head_;
        head_ = head_->next_;

        return reinterpret_cast<T*>(data->buffer_);
    }

    void deallocate(T* ptr, size_t sz) {
        Node* node = reinterpret_cast<Node*>(ptr);
        node->next_ = head_;
        head_ = node;
    }

private:
    struct alignas(std::max(alignof(T), alignof(void*))) Node {
        union {
            char buffer_[sizeof(T) * ChunkSize];
            Node* next_;
        };
    };
    Node* head_;
    Node* buffer_;
};
