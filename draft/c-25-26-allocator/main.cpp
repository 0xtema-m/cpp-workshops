
#include <chrono>
#include <iostream>
#include <vector>
#include <list>

#include "chunk_allocator.h"
#include "default_alloc.h"
#include "stack_allocator.h"

int main() {
    // std::list<int, ChunkAllocator<int, 1000000> > l;
    std::list<int> l;

    auto start = std::chrono::steady_clock::now();
    for (int i = 0; i < 1000000; ++i) {
        l.push_back(i);
    }
    auto end = std::chrono::steady_clock::now();
    const std::chrono::duration<double> elapsed_seconds{end - start};

    std::cout << elapsed_seconds;
}
