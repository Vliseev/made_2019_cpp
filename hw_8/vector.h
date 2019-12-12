//
// Created by user on 09.12.2019.
//

#pragma once
#include <iterator>
#include <memory>

template <class T>
struct Iterator {
   public:
    using iterator_category = std::random_access_iterator_tag;
    using value_type = T;
    using difference_type = std::ptrdiff_t;
    using pointer = T*;
    using reference = T&;
    using const_reference = const T&;

   private:
    pointer ptr_;

   public:
    explicit Iterator(pointer ptr) : ptr_(ptr){};
    bool operator==(const Iterator<T>& other) const {
        return ptr_ == other.ptr_;
    }

    bool operator!=(const Iterator<T>& other) const {
        return !(*this == other);
    }

    reference operator*() const {
        return *ptr_;
    }

    Iterator& operator++() {
        ++ptr_;
        return *this;
    }

    Iterator& operator--() {
        --ptr_;
        return *this;
    }

    Iterator operator++(int) const {
        Iterator tmp = *this;
        ++ptr_;
        return tmp;
    }

    Iterator operator--(int) const {
        Iterator tmp = *this;
        --ptr_;
        return tmp;
    }

    Iterator operator+(difference_type n) const {
        return Iterator(ptr_ + n);
    }

    Iterator operator-(difference_type n) const {
        return Iterator(ptr_ - n);
    }

    reference operator[](difference_type n) {
        return *(ptr_ + n);
    }

    const_reference operator[](difference_type n) const {
        return *(ptr_ + n);
    }
};

template <class T, class Allocator = std::allocator<T>>
class Vector {
    using value_type = T;
    using pointer = T*;
    using reference = T&;
    using const_reference = const T&;
    using iterator = Iterator<T>;
    using reverse_iterator = std::reverse_iterator<iterator>;

   public:
    typedef typename Allocator::template rebind<T>::other Tp_alloc_type;

    Vector()
        : m_beg_(nullptr),
          m_end_(nullptr),
          m_end_storage_(nullptr),
          allocator_(){};
    explicit Vector(size_t n);
    Vector(std::initializer_list<T> l);
    Vector(const Vector& other);
    Vector(Vector&& other) noexcept;

    Vector& operator=(const Vector& other);
    Vector& operator=(Vector&& other) noexcept;

    ~Vector();

    void PushBack(const T& value);
    void PushBack(T&&);
    void PopBack();
    bool Empty();
    void Clear();
    size_t Size();
    void Resize(size_t new_size);
    void Reserve(size_t new_size);

    reference operator[](size_t n);
    const_reference operator[](size_t n) const;

    iterator begin() {
        return iterator(m_beg_);
    }

    iterator end() {
        return iterator(m_end_);
    }

    reverse_iterator rbegin() {
        return reverse_iterator(end());
    }

    reverse_iterator rend() {
        return reverse_iterator(begin());
    }

   private:
    pointer m_beg_;
    pointer m_end_;
    pointer m_end_storage_;
    Tp_alloc_type allocator_;

    void ReallocBuf(size_t new_cap);
    void ConstructRange(pointer begin, pointer end);
    void DestroyRange(pointer begin, pointer end);
    void CopyRange(pointer begin, pointer end, pointer dest);
    void MoveRange(pointer begin, pointer end, pointer dest);
};

template <class T, class Allocator>
Vector<T, Allocator>::Vector(size_t n) : allocator_() {
    m_beg_ = static_cast<pointer>(allocator_.allocate(sizeof(T) * n));
    m_end_storage_ = m_beg_ + n;
    m_end_ = m_end_storage_;

    //    std::uninitialized_fill_n(m_beg_, m_end_, T());
    ConstructRange(m_beg_, m_end_);
}

template <class T, class Allocator>
Vector<T, Allocator>::Vector(std::initializer_list<T> l) {
    m_beg_ = static_cast<pointer>(allocator_.allocate(sizeof(T) * l.size()));
    m_end_storage_ = m_beg_ + l.size();
    m_end_ = m_end_storage_;

    auto beg_p = m_beg_;
    for (auto& el : l) {
        ::new (beg_p++) T(el);
    }
}

template <class T, class Allocator>
void Vector<T, Allocator>::ReallocBuf(size_t new_cap) {
    pointer old_beg = m_beg_;
    pointer old_end = m_end_;
    pointer old_end_storage = m_end_storage_;

    m_beg_ = static_cast<pointer>(allocator_.allocate(sizeof(T) * new_cap));
    m_end_storage_ = m_beg_ + new_cap;
    m_end_ = m_beg_ + std::distance(old_beg, old_end);

    ConstructRange(m_beg_, m_end_);
    MoveRange(old_beg, old_end, m_beg_);
    DestroyRange(old_beg, old_end);
    //    std::copy(old_beg, old_end, m_beg_);
    //    std::destroy_n(old_beg, std::distance(old_beg, old_end));

    allocator_.deallocate(old_beg, std::distance(old_beg, old_end_storage));
}
template <class T, class Allocator>
void Vector<T, Allocator>::PushBack(const T& value) {
    if (m_end_ != m_end_storage_) {
        ::new ((void*)m_end_++) T(value);
        return;
    } else {
        ReallocBuf(std::distance(m_beg_, m_end_storage_) * 2 + 1);
        ::new ((void*)m_end_++) T(value);
        return;
    }
}
template <class T, class Allocator>
void Vector<T, Allocator>::PushBack(T&& value) {
    if (m_end_ != m_end_storage_) {
        ::new ((void*)m_end_++) T(std::move(value));
        return;
    } else {
        ReallocBuf(std::distance(m_beg_, m_end_storage_) * 2 + 1);
        ::new ((void*)m_end_++) T(std::move(value));
        return;
    }
}

template <class T, class Allocator>
Vector<T, Allocator>::~Vector() {
    //    std::destroy_n(m_beg_, m_end_);
    DestroyRange(m_beg_, m_end_);
    allocator_.deallocate(m_beg_, std::distance(m_beg_, m_end_storage_));
}

template <class T, class Allocator>
void Vector<T, Allocator>::ConstructRange(Vector::pointer begin,
                                          Vector::pointer end) {
    while (begin != end) {
        new (begin++) T();
    }
}

template <class T, class Allocator>
void Vector<T, Allocator>::DestroyRange(Vector::pointer begin,
                                        Vector::pointer end) {
    while (begin != end) {
        (begin++)->~T();
    }
}

template <class T, class Allocator>
void Vector<T, Allocator>::CopyRange(Vector::pointer begin, Vector::pointer end,
                                     Vector::pointer dest) {
    while (begin != end) {
        new (dest++) T(*(begin++));
    }
}

template <class T, class Allocator>
void Vector<T, Allocator>::MoveRange(Vector::pointer begin, Vector::pointer end,
                                     Vector::pointer dest) {
    while (begin != end) {
        new (dest++) T(std::move(*(begin++)));
    }
}

template <class T, class Allocator>
void Vector<T, Allocator>::PopBack() {
    m_end_--;
    m_end_->~T();
}

template <class T, class Allocator>
bool Vector<T, Allocator>::Empty() {
    return m_beg_ == m_end_;
}

template <class T, class Allocator>
size_t Vector<T, Allocator>::Size() {
    return std::distance(m_beg_, m_end_);
}

template <class T, class Allocator>
void Vector<T, Allocator>::Clear() {
    DestroyRange(m_beg_, m_end_);
    m_end_ = m_beg_;
}

template <class T, class Allocator>
void Vector<T, Allocator>::Resize(size_t new_size) {
    size_t old_size = std::distance(m_beg_, m_end_);
    size_t old_cap = std::distance(m_beg_, m_end_storage_);
    if (old_size >= new_size) {
        DestroyRange(m_beg_ + new_size, m_end_);
        m_end_ = m_beg_ + new_size;
        return;
    }
    if (new_size <= old_cap) {
        ConstructRange(m_end_, m_beg_ + new_size);
        m_end_ = m_beg_ + new_size;
        return;
    }
    size_t new_cap = new_size;

    if (new_cap < old_size * 2) {
        new_cap = old_size * 2;
    }
    ReallocBuf(new_cap);
    ConstructRange(m_beg_ + old_size, m_beg_ + new_size);
}

template <class T, class Allocator>
typename Vector<T, Allocator>::reference
    Vector<T, Allocator>::operator[](size_t n) {
    return *(this->m_beg_ + n);
}

template <class T, class Allocator>
typename Vector<T, Allocator>::const_reference
    Vector<T, Allocator>::operator[](size_t n) const {
    return *(this->m_beg_ + n);
}
template <class T, class Allocator>
void Vector<T, Allocator>::Reserve(size_t new_size) {
    size_t cap = std::distance(m_beg_, m_end_storage_);
    if (cap < new_size)
        ReallocBuf(new_size);
}
template <class T, class Allocator>
Vector<T, Allocator>::Vector(const Vector& other) {
    auto other_size = std::distance(other.m_beg_, other.m_end_);

    m_beg_ = static_cast<pointer>(allocator_.allocate(sizeof(T) * other_size));
    m_end_storage_ = m_beg_ + other_size;
    m_end_ = m_end_storage_;

    auto beg_p = m_beg_;
    for (size_t i = 0; i < other_size; ++i) {
        ::new (beg_p++) T(other[i]);
    }
}
template <class T, class Allocator>
Vector<T, Allocator>::Vector(Vector&& other) noexcept {
    m_beg_ = other.m_beg_;
    m_end_ = other.m_end_;
    m_end_storage_ = other.m_end_storage_;

    other.m_beg_ = other.m_end_ = other.m_end_storage_ = nullptr;
}
template <class T, class Allocator>
Vector<T, Allocator>& Vector<T, Allocator>::operator=(const Vector& other) {
    if (std::addressof(other) == this)
        return *this;

    auto other_size = std::distance(other.m_beg_, other.m_end_);

    m_beg_ = static_cast<pointer>(allocator_.allocate(sizeof(T) * other_size));
    m_end_storage_ = m_beg_ + other_size;
    m_end_ = m_end_storage_;

    auto beg_p = m_beg_;
    for (size_t i = 0; i < other_size; ++i) {
        ::new (beg_p++) T(other[i]);
    }
    return *this;
}
template <class T, class Allocator>
Vector<T, Allocator>& Vector<T, Allocator>::operator=(Vector&& other) noexcept {
    if (std::addressof(other) == this)
        return *this;

    m_beg_ = other.m_beg_;
    m_end_ = other.m_end_;
    m_end_storage_ = other.m_end_storage_;

    other.m_beg_ = other.m_end_ = other.m_end_storage_ = nullptr;

    return *this;
}
