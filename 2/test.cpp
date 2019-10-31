// This is an open source non-commercial project. Dear PVS-Studio, please check
// it. PVS-studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "TestRunner.h"
#include "linear_allocator.h"

void SimpleTest() {
    static const int kAllocSize = 20;
    char *pointer;
    LinearAllocator allocator(kAllocSize);

    pointer = allocator.Alloc(15);
    ASSERT(pointer != nullptr);

    allocator.Reset();

    pointer = allocator.Alloc(20);
    ASSERT(pointer != nullptr);
}

void Test1() {
    static const int kAllocSize = 20;
    char *pointer;
    LinearAllocator allocator(kAllocSize);

    pointer = allocator.Alloc(20);
    ASSERT(pointer != nullptr);
    pointer = allocator.Alloc(1);
    ASSERT(pointer == nullptr);

    allocator.Reset();

    pointer = allocator.Alloc(18);
    ASSERT(pointer != nullptr);

    pointer = allocator.Alloc(3);
    ASSERT(pointer == nullptr);

    pointer = allocator.Alloc(2);
    ASSERT(pointer != nullptr);
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, SimpleTest);
    RUN_TEST(tr, Test1);
    return 0;
}