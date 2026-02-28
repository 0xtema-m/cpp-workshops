#include <iostream>
#include <vector>
#include <list>
#include <chrono>
#include <string>

#include "chunk_alloc.h"
#include "default_alloc.h"
#include "stack_alloc.h"

/*
[] -> [] -> [] -> [] -> []
*/

int main() {
    std::list<int> std_list;
    std::list<int, ChunkAlloc<int, 1>> my_list;
    std::list<int, ChunkPreallocatedAlloc<int, 1000000, 1>> my_list_prealloc;
    {
        auto start = std::chrono::steady_clock::now();
        for (int i = 0; i < 1000000; ++i) {
            std_list.push_back(i);
        }
        // for (int i = 0; i < 1000; ++i) {
        //     for (int j = 0; j < 10000; ++j) {
        //         std_list.push_back(j);
        //     }
        //     for (int j = 0; j < 10000; ++j) {
        //         std_list.pop_back();
        //     }
        // }
        auto end = std::chrono::steady_clock::now();
        const std::chrono::duration<double> dur{end - start};
        std::cout << dur << std::endl;
    }
    {
        auto start = std::chrono::steady_clock::now();
        for (int i = 0; i < 1000000; ++i) {
            my_list.push_back(i);
        }
        // for (int i = 0; i < 1000; ++i) {
        //     for (int j = 0; j < 10000; ++j) {
        //         my_list.push_back(j);
        //     }
        //     for (int j = 0; j < 10000; ++j) {
        //         my_list.pop_back();
        //     }
        // }
        auto end = std::chrono::steady_clock::now();
        const std::chrono::duration<double> dur{end - start};
        std::cout << dur << std::endl;
    }
    {
        auto start = std::chrono::steady_clock::now();
        for (int i = 0; i < 1000000; ++i) {
            my_list_prealloc.push_back(i);
        }
        // for (int i = 0; i < 1000; ++i) {
        //     for (int j = 0; j < 10000; ++j) {
        //         my_list_prealloc.push_back(j);
        //     }
        //     for (int j = 0; j < 10000; ++j) {
        //         my_list_prealloc.pop_back();
        //     }
        // }
        auto end = std::chrono::steady_clock::now();
        const std::chrono::duration<double> dur{end - start};
        std::cout << dur << std::endl;
    }
}
