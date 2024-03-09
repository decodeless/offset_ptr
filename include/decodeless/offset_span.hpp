// Copyright (c) 2024 Pyarelal Knowles, MIT License

#pragma once

#include <decodeless/offset_ptr.hpp>

#if __has_include(<span>)
    #include <span>
#endif

#if __has_include(<ranges>)
    #include <ranges>
#endif

#ifndef ENABLE_OFFSET_PTR_TRANSLATE
#define ENABLE_OFFSET_PTR_TRANSLATE 0
#endif

namespace decodeless {

// std::span equivalent but implemented with an offset_ptr
template <class T> class offset_span {
public:
    using value_type = T;
    using size_type = std::size_t;
    using iterator = T*;
    using reference = T&;
    offset_span() = default;
    offset_span(T* ptr, size_type count) : m_data(ptr), m_size(count) {}

#ifdef __cpp_lib_span
    offset_span(const std::span<T>& span)
        : offset_span(span.data(), span.size()) {}
    operator std::span<T>() const { return {m_data.get(), m_size}; }
    std::span<T> subspan(size_type offset) const {
        return {m_data.get() + offset, m_size - offset};
    }
    #if ENABLE_OFFSET_PTR_TRANSLATE
    std::span<T> translate(const void* srcBase, void* dstBase) const {
        return {m_data.translate(srcBase, dstBase), m_size};
    }
    std::span<const T> translate(const void* srcBase,
                                 const void* dstBase) const {
        return {m_data.translate(srcBase, dstBase), m_size};
    }
    #endif
#endif

#ifdef __cpp_lib_ranges
    template <class Range>
        requires std::ranges::contiguous_range<Range>
    offset_span(Range& range)
        : offset_span(std::ranges::data(range), std::ranges::size(range)) {}
#endif

    iterator data() const { return m_data.get(); }
    const size_type& size() const { return m_size; }
    iterator begin() const { return data(); }
    iterator end() const { return data() + m_size; }
    reference operator[](size_type pos) const { return begin()[pos]; }
    reference front() const { return *begin(); }
    reference back() const { return *std::prev(end()); }

private:
    offset_ptr<T> m_data;
    size_type m_size = 0;
};

template <typename Range>
offset_span(Range&& range) -> offset_span<std::remove_reference_t<
    decltype(*std::ranges::data(std::forward<Range>(range)))>>;

} // namespace decodeless
