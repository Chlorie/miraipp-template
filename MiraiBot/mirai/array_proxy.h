#pragma once

#include <array>
#include <vector>

namespace mirai
{
    /**
     * \brief A non-owning view to a contiguous range of elements, use std::span when C++20 comes out
     * \tparam T Type of the array elements
     */
    template <typename T>
    class ArrayProxy final
    {
    private:
        const T* data_ = nullptr;
        size_t size_ = 0;
    public:
        constexpr ArrayProxy() = default;
        constexpr ArrayProxy(const T& value) :data_(&value), size_(1) {}
        constexpr ArrayProxy(const T* data, const size_t size) : data_(data), size_(size) {}
        template <size_t N>
        constexpr ArrayProxy(const T(&array)[N]) : data_(array), size_(N) {}
        constexpr ArrayProxy(const std::initializer_list<T> list) : data_(&*list.begin()), size_(list.size()) {}
        template <size_t N>
        constexpr ArrayProxy(const std::array<T, N>& array) : data_(array.data()), size_(N) {}
        ArrayProxy(const std::vector<T>& vec) :data_(vec.data()), size_(vec.size()) {}

        constexpr const T* data() const { return data_; }
        constexpr size_t size() const { return size_; }
        constexpr const T& operator[](const size_t index) const { return data_[index]; }
        constexpr bool empty() const { return size_ == 0; }

        constexpr const T* begin() const { return data_; }
        constexpr const T* end() const { return data_ + size_; }
        constexpr const T* cbegin() const { return data_; }
        constexpr const T* cend() const { return data_ + size_; }
    };
}