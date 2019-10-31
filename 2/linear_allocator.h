#ifndef INC_2__ALLOCATOR_H_
#define INC_2__ALLOCATOR_H_

#include <cstdlib>
class LinearAllocator {
    size_t max_size_;
    size_t size_;
    char* buf_{};

   public:
    explicit LinearAllocator(size_t max_size);
    char* Alloc(size_t size);
    void Reset();
    ~LinearAllocator();
};

#endif    // INC_2__ALLOCATOR_H_
