// Copyright (c) 2024 Pyarelal Knowles, MIT License

#pragma once

#include <cstddef>

#ifndef ENABLE_OFFSET_PTR_TRANSLATE
#define ENABLE_OFFSET_PTR_TRANSLATE 0
#endif

namespace nodecode {

// Pointer with storage relative to its current memory location
template <class T> class offset_ptr {
public:
    using value_type = T;
    using offset_type = std::ptrdiff_t;
    using size_type = std::size_t;
    offset_ptr() = default;
    offset_ptr(const offset_ptr& other) : offset_ptr(other.get()) {}
    offset_ptr(T* ptr) { set(ptr); }
    T* get() const {
        return m_offset == NullValue ? nullptr
                                     : reinterpret_cast<T*>(base_() + m_offset);
    }
    void set(T* ptr) {
        m_offset = (ptr == nullptr)
                       ? NullValue
                       : (reinterpret_cast<offset_type>(ptr) - base_());
    }
    bool operator==(const offset_ptr& other) const {
        return get() == other.get();
    }
    bool operator!=(const offset_ptr& other) const {
        return get() != other.get();
    }
    offset_ptr& operator=(const offset_ptr& other) {
        set(other.get());
        return *this;
    }
    T& operator[](size_type pos) const { return get()[pos]; }
    T& operator*() const { return *get(); }
    T* operator->() const { return get(); }
    operator bool() const { return m_offset != NullValue; }

    #if ENABLE_OFFSET_PTR_TRANSLATE
    T* translate(const void* srcBase, void* dstBase) const {
        return reinterpret_cast<T*>(reinterpret_cast<uintptr_t>(get()) -
                                    reinterpret_cast<uintptr_t>(srcBase) +
                                    reinterpret_cast<uintptr_t>(dstBase));
    }
    const T* translate(const void* srcBase, const void* dstBase) const {
        return reinterpret_cast<const T*>(reinterpret_cast<uintptr_t>(get()) -
                                          reinterpret_cast<uintptr_t>(srcBase) +
                                          reinterpret_cast<uintptr_t>(dstBase));
    }
    #endif

private:
    offset_type base_() const { return reinterpret_cast<offset_type>(this); }

    // Considered null when the offset is one byte, to allow pointing to self.
    static const offset_type NullValue = 1;

    offset_type m_offset = NullValue;
};

} // namespace nodecode