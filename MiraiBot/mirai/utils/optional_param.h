#pragma once

#include <utility>

namespace mirai::utils
{
    /**
     * \brief Optional parameter type which wraps a pointer
     * \tparam T Type of the parameter
     */
    template <typename T>
    class OptionalParam final
    {
    private:
        const T* ptr_ = nullptr;
    public:
        OptionalParam() = default;
        OptionalParam(std::nullptr_t) {}
        OptionalParam(const T& value) :ptr_(&value) {}

        constexpr const T& operator*() const { return *ptr_; }
        constexpr const T* operator->() const { return ptr_; }
        constexpr const T* pointer() const { return ptr_; }
        constexpr explicit operator bool() const { return ptr_; }
        constexpr bool has_value() const { return ptr_; }
        constexpr const T& value() const { return *ptr_; }
        template <typename U>
        constexpr const T& value_or(U&& value) const { return ptr_ ? *ptr_ : std::forward<U>(value); }
    };
}
