#pragma once

#include <variant>

namespace mirai::utils
{
    namespace detail
    {
        template <typename Var, typename T> struct in_variant {};
        template <typename... Ts, typename T>
        struct in_variant<std::variant<Ts...>, T> :
            std::bool_constant<(std::is_same_v<Ts, T> || ...)> {};
        template <typename Var, typename T> constexpr bool in_variant_v = in_variant<Var, T>::value;
    }

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

        template <typename T, typename F, typename Obj>
        static void dispatch_impl(Obj&& obj, F&& func)
        {
            std::forward<Obj>(obj).apply([&func](auto&& data)
            {
                if constexpr (std::is_void_v<T>) // Accept all
                {
                    if constexpr (std::is_invocable_v<F&&, decltype(data)>)
                        std::forward<F>(func)(data);
                }
                else // Use the given T
                {
                    if constexpr (std::is_same_v<decltype(data), T>)
                        std::forward<F>(func)(data);
                }
            });
        }
    public:
        using Type = EnumType;
        using Variant = VariantType;

        /**
         * \brief Construct a default variant
         */
        VariantWrapper() = default;

        /**
         * \brief Construct a variant with any of the underlying types
         * \tparam T Type of the object
         * \param object The object
         */
        template <typename T,
            std::enable_if_t<detail::in_variant_v<VariantType, std::decay_t<T>>>* = nullptr>
        VariantWrapper(T&& object): data_(std::forward<T>(object)) {}

        /**
         * \brief Copy a variant
         * \param variant The other variant
         */
        VariantWrapper(const Variant& variant) : data_(variant) {}

        /**
         * \brief Move a variant
         * \param variant The other variant
         */
        VariantWrapper(Variant&& variant) : data_(std::move(variant)) {}

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
         * \brief Check if two variants are equal
         * \param lhs The first variant
         * \param rhs The second variant
         * \return The result
         */
        friend bool operator==(const VariantWrapper& lhs, const VariantWrapper& rhs) { return lhs.data_ == rhs.data_; }

        /**
         * \brief Check if two variants are not equal
         * \param lhs The first variant
         * \param rhs The second variant
         * \return The result
         */
        friend bool operator!=(const VariantWrapper& lhs, const VariantWrapper& rhs) { return !(lhs == rhs); }

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

        /**
         * \brief Call a function object on the variant if the type of the variant matches
         * \tparam T The type you want to dispatch to, leave as void to accept all types that
         * is valid to call the function object
         * \tparam F The type of the function object, don't specify this for auto deduction
         * \param func The function object
         */
        template <typename T = void, typename F = void>
        void dispatch(F&& func) { dispatch_impl<T>(*this, std::forward<F>(func)); }

        /**
         * \brief Call a function object on the variant if the type of the variant matches
         * \tparam T The type you want to dispatch to, leave as void to accept all types that
         * is valid to call the function object
         * \tparam F The type of the function object, don't specify this for auto deduction
         * \param func The function object
         */
        template <typename T = void, typename F = void>
        void dispatch(F&& func) const { dispatch_impl<T>(*this, std::forward<F>(func)); }
    };
}
