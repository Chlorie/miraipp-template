#pragma once

#include <variant>
#include "meta.h"

namespace mirai::utils
{
    /**
     * \brief Provide static member for whether a callable is suitable
     * for visiting a specific variant type
     * \tparam F Type of the callable object
     * \tparam V Type of the variant
     */
    template <typename F, typename V> struct is_variant_visitable : std::false_type {};
    template <typename F, typename... Ts> struct is_variant_visitable<F, std::variant<Ts...>> :
        std::bool_constant<is_visitable_v<F, Ts...>> {};

    /**
     * \brief Check whether a callable is suitable for visiting a specific variant type
     * \tparam F Type of the callable object
     * \tparam V Type of the variant
     */
    template <typename F, typename V>
    constexpr bool is_variant_visitable_v = is_variant_visitable<F, V>::value;

    /**
     * \brief A wrapper for easier access of std::variant, providing member functions
     * for query the type index and visiting the variant
     * \tparam VariantType The std::variant type to wrap
     * \tparam EnumType The enum type corresponding to the types consisting the variant
     */
    template <typename VariantType, typename EnumType>
    class VariantWrapper
    {
    private:
        template <typename F, typename D>
        using visit_result_t = decltype((std::visit(std::declval<F&&>(), std::declval<D>())));

        VariantType data_;
    public:
        using Type = EnumType;
        using Variant = VariantType;

        /**
         * \brief Construct a default variant
         */
        VariantWrapper() = default;

        /**
         * \brief Copy a variant
         * \param variant The other variant
         */
        VariantWrapper(const Variant& variant) :data_(variant) {}

        /**
         * \brief Move a variant
         * \param variant The other variant
         */
        VariantWrapper(Variant&& variant) :data_(std::move(variant)) {}

        /**
         * \brief Copy a variant
         * \param variant The other variant
         * \returns Reference to this object
         */
        VariantWrapper& operator=(const Variant& variant)
        {
            data_ = variant;
            return *this;
        }

        /**
         * \brief Move a variant
         * \param variant The other variant
         * \returns Reference to this object
         */
        VariantWrapper& operator=(Variant&& variant)
        {
            data_ = std::move(variant);
            return *this;
        }

        /**
         * \brief Get which type of object the variant is current holding
         * \return The type
         */
        Type type() const { return Type(data_.index()); }

        /**
         * \brief Get the data with a specific type, throw if the types don't match
         * \tparam T The type (in enum) of the variation to get
         * \return A reference to the object
         */
        template <Type T> auto& get() { return std::get<size_t(T)>(data_); }

        /**
         * \brief Get the data with a specific type, throw if the types don't match
         * \tparam T The type (in enum) of the variation to get
         * \return A const reference to the object
         */
        template <Type T> const auto& get() const { return std::get<size_t(T)>(data_); }

        /**
         * \brief Get the data with a specific type, throw if the types don't match
         * \tparam T The type of the variation to get
         * \return A reference to the object
         */
        template <typename T> T& get() { return std::get<T>(data_); }

        /**
         * \brief Get the data with a specific type, throw if the types don't match
         * \tparam T The type of the variation to get
         * \return A const reference to the object
         */
        template <typename T> const T& get() const { return std::get<T>(data_); }

        /**
         * \brief Get the data pointer with a specific type,
         * returns nullptr if the types don't match
         * \tparam T The type (in enum) of the variation to get
         * \return A pointer to the object
         */
        template <Type T> auto get_if() { return std::get_if<size_t(T)>(&data_); }

        /**
         * \brief Get the data pointer with a specific type,
         * returns nullptr if the types don't match
         * \tparam T The type (in enum) of the variation to get
         * \return A pointer to the const object
         */
        template <Type T> auto get_if() const { return std::get_if<size_t(T)>(&data_); }

        /**
         * \brief Get the data pointer with a specific type,
         * returns nullptr if the types don't match
         * \tparam T The type of the variation to get
         * \return A pointer to the object
         */
        template <typename T> T* get_if() { return std::get_if<T>(&data_); }

        /**
         * \brief Get the data pointer with a specific type,
         * returns nullptr if the types don't match
         * \tparam T The type of the variation to get
         * \return A pointer to the const object
         */
        template <typename T> const T* get_if() const { return std::get_if<T>(&data_); }

        /**
         * \brief Get a reference of the underlying variant data
         * \return The result
         */
        Variant& data() { return data_; }

        /**
         * \brief Get a const reference of the underlying variant data
         * \return The result
         */
        const Variant& data() const { return data_; }

        /**
         * \brief Apply a callable object to the variant (calling std::visit)
         * \tparam F Type of the callable object
         * \param func The callable object
         * \return The result
         */
        template <typename F, typename = visit_result_t<F, Variant&>>
        decltype(auto) apply(F&& func)
        {
            return std::visit(std::forward<F>(func), data_);
        }

        /**
         * \brief Apply a callable object to the variant (calling std::visit)
         * \tparam F Type of the callable object
         * \param func The callable object
         * \return The result
         */
        template <typename F, typename = visit_result_t<F, const Variant&>>
        decltype(auto) apply(F&& func) const
        {
            return std::visit(std::forward<F>(func), data_);
        }
    };
}
