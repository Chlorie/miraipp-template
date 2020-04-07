#pragma once

#include <variant>
#include <nlohmann/json.hpp>

namespace mirai
{
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
        using enable_if_visitable_t = std::void_t<decltype(std::visit(
            std::declval<F&&>(), std::declval<D>()))>;

        VariantType data_;
    public:
        using Type = EnumType;
        using Variant = VariantType;

        VariantWrapper() = default;
        VariantWrapper(const VariantType& variant) :data_(variant) {}
        VariantWrapper(VariantType&& variant) :data_(std::move(variant)) {}
        VariantWrapper& operator=(const VariantType& variant)
        {
            data_ = variant;
            return *this;
        }
        VariantWrapper& operator=(VariantType&& variant)
        {
            data_ = std::move(variant);
            return *this;
        }

        Type type() const { return Type(data_.index()); }
        template <Type T> auto& get() { return std::get<size_t(T)>(data_); }
        template <Type T> const auto& get() const { return std::get<size_t(T)>(data_); }
        template <typename T> T& get() { return std::get<T>(data_); }
        template <typename T> const T& get() const { return std::get<T>(data_); }

        VariantType& data() { return data_; }
        const VariantType& data() const { return data_; }

        template <typename F, typename = enable_if_visitable_t<F, VariantType&>>
        decltype(auto) apply(F&& func)
        {
            return std::visit(std::forward<F>(func), data_);
        }
        template <typename F, typename = enable_if_visitable_t<F, const VariantType&>>
        decltype(auto) apply(F&& func) const
        {
            return std::visit(std::forward<F>(func), data_);
        }
    };
}
