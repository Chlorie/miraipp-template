#pragma once

#include <array>
#include <vector>

namespace mirai::utils
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
        /**
         * \brief Construct a default empty array proxy
         */
        constexpr ArrayProxy() = default;

        /**
         * \brief Construct an array proxy containing only one value
         * \param value The value
         */
        constexpr ArrayProxy(const T& value) : data_(&value), size_(1) {}

        /**
         * \brief Construct an array proxy from pointer and size
         * \param data The pointer to the beginning of the array
         * \param size The size of the array
         */
        constexpr ArrayProxy(const T* data, const size_t size) : data_(data), size_(size) {}

        /**
         * \brief Construct an array proxy from a C-style array
         * \tparam N The size of the C-style array (can be auto deduced)
         * \param array The array
         */
        template <size_t N>
        constexpr ArrayProxy(const T (&array)[N]) : data_(array), size_(N) {}

        /**
         * \brief Construct an array proxy from a brace initializer list
         * \param list The initializer list
         */
        constexpr ArrayProxy(const std::initializer_list<T> list) : data_(&*list.begin()), size_(list.size()) {}

        /**
         * \brief Construct an array proxy from a C++ std::array
         * \tparam N The size of the array (can be auto deduced)
         * \param array The C++ std::array
         */
        template <size_t N>
        constexpr ArrayProxy(const std::array<T, N>& array) : data_(array.data()), size_(N) {}

        /**
         * \brief Construct an array proxy from a vector
         * \param vec The vector
         */
        ArrayProxy(const std::vector<T>& vec) : data_(vec.data()), size_(vec.size()) {}

        /**
         * \brief Get a pointer to the data
         * \return The pointer
         */
        constexpr const T* data() const { return data_; }

        /**
         * \brief Get the size of the array
         * \return The size
         */
        constexpr size_t size() const { return size_; }

        /**
         * \brief Indexing the array
         * \param index The index
         * \return A const reference to the object
         */
        constexpr const T& operator[](const size_t index) const { return data_[index]; }

        /**
         * \brief Check whether the array is empty
         * \return The result
         */
        constexpr bool empty() const { return size_ == 0; }

        /**
         * \brief Get a pointer to the beginning of the array
         * \return The pointer
         */
        constexpr const T* begin() const { return data_; }

        /**
         * \brief Get a pointer to the end of the array (one past the last element)
         * \return The pointer
         */
        constexpr const T* end() const { return data_ + size_; }

        /**
         * \brief Get a pointer to the beginning of the array
         * \return The pointer
         */
        constexpr const T* cbegin() const { return data_; }

        /**
         * \brief Get a pointer to the end of the array (one past the last element)
         * \return The pointer
         */
        constexpr const T* cend() const { return data_ + size_; }
    };
}
