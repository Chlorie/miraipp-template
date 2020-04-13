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
        /**
         * \brief Construct a default optional parameter (empty)
         */
        OptionalParam() = default;

        /**
         * \brief Construct an empty optional parameter
         */
        OptionalParam(std::nullptr_t) {}

        /**
         * \brief Construct a non-empty optional parameter with an object
         * \param value A const reference to the real parameter
         */
        OptionalParam(const T& value) : ptr_(&value) {}

        /**
         * \brief Get a pointer to the object, will be nullptr if empty
         * \return The pointer
         */
        constexpr const T& operator*() const { return *ptr_; }

        /**
         * \brief Get a pointer to the object, will be nullptr if empty
         * \return The pointer
         */
        constexpr const T* operator->() const { return ptr_; }

        /**
         * \brief Get a pointer to the object, will be nullptr if empty
         * \return The pointer
         */
        constexpr const T* pointer() const { return ptr_; }

        /**
         * \brief Cast to bool to integrate with if statements
         */
        constexpr explicit operator bool() const { return ptr_; }

        /**
         * \brief Whether the optional parameter has a value
         * \return The result
         */
        constexpr bool has_value() const { return ptr_; }

        /**
         * \brief Get a reference to the object, should only be used
         * when the optional parameter has value
         * \return The reference
         */
        constexpr const T& value() const { return *ptr_; }

        /**
         * \brief Return the value if presents, return the given default value when empty
         * \param value The default value
         * \return The result
         */
        template <typename U>
        constexpr T value_or(T value) const { return ptr_ ? *ptr_ : std::move(value); }
    };
}
