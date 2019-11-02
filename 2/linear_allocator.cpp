// This is an open source non-commercial project. Dear PVS-Studio, please check
// it. PVS-studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "linear_allocator.h"
LinearAllocator::LinearAllocator(size_t max_size)
    : max_size_(max_size), size_(0) {
    buf_ = new char[max_size];
}
char* LinearAllocator::Alloc(size_t size) {
    if (size + size_ <= max_size_) {
        char* alloc_ptr = buf_ + size_;
        size_ += size;
        return alloc_ptr;
    } else {
        return nullptr;
    }
}
void LinearAllocator::Reset() {
    size_ = 0;
}
LinearAllocator::~LinearAllocator() {
    delete[] buf_;
}
